/**
 *
 *  @file topology_test.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/cgride/graph
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Cgride
 *
 */
#include <cassert>

#include <cgride/graph/topology.hpp>

int main()
{
  {
    cgride::graph::Graph graph;

    auto errors = cgride::graph::validate_graph_errors(graph);

    assert(errors.size() == 1);
    assert(errors[0].code() == cgride::core::ErrorCode::InvalidArgument);
    assert(errors[0].message() == "Graph has no tasks.");

    auto result = cgride::graph::validate_graph(graph);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
  }

  {
    cgride::graph::Graph graph("valid");

    auto &prepare = graph.task(
        cgride::graph::TaskId::from_string("prepare"),
        cgride::graph::TaskKind::Prepare);

    auto &compile = graph.task(
        cgride::graph::TaskId::from_string("compile:main.cpp"),
        cgride::graph::TaskKind::Compile);

    auto &link = graph.task(
        cgride::graph::TaskId::from_string("link:app"),
        cgride::graph::TaskKind::Link);

    compile.depends_on(prepare.id());
    link.depends_on(compile.id());

    auto errors = cgride::graph::validate_graph_errors(graph);

    assert(errors.empty());

    auto result = cgride::graph::validate_graph(graph);

    assert(result);
  }

  {
    cgride::graph::Graph graph("invalid-task");

    graph.add_task(cgride::graph::Task{});

    auto errors = cgride::graph::validate_graph_errors(graph);

    assert(!errors.empty());
    assert(errors[0].code() == cgride::core::ErrorCode::InvalidArgument);
    assert(errors[0].message() == "Graph contains an invalid task.");
  }

  {
    cgride::graph::Graph graph("duplicate-ids");

    graph.task(
        cgride::graph::TaskId::from_string("compile"),
        cgride::graph::TaskKind::Compile);

    graph.task(
        cgride::graph::TaskId::from_string("compile"),
        cgride::graph::TaskKind::Compile);

    auto errors = cgride::graph::validate_graph_errors(graph);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidArgument &&
          error.message() == "Graph contains duplicate task ids." &&
          error.detail().has_value() &&
          error.detail().value() == "compile")
      {
        found = true;
      }
    }

    assert(found);
  }

  {
    cgride::graph::Graph graph("invalid-dependency");

    auto &task = graph.task(
        cgride::graph::TaskId::from_string("link"),
        cgride::graph::TaskKind::Link);

    task.depends_on(cgride::graph::TaskId{});

    auto errors = cgride::graph::validate_graph_errors(graph);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidArgument &&
          error.message() == "Task contains an invalid dependency." &&
          error.detail().has_value() &&
          error.detail().value() == "link")
      {
        found = true;
      }
    }

    assert(found);
  }

  {
    cgride::graph::Graph graph("unknown-dependency");

    auto &link = graph.task(
        cgride::graph::TaskId::from_string("link"),
        cgride::graph::TaskKind::Link);

    link.depends_on(cgride::graph::TaskId::from_string("compile"));

    auto errors = cgride::graph::validate_graph_errors(graph);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::NotFound &&
          error.message() == "Task dependency refers to an unknown task." &&
          error.detail().has_value() &&
          error.detail().value() == "compile")
      {
        found = true;
      }
    }

    assert(found);
  }

  {
    cgride::graph::Graph graph("self-dependency");

    auto &task = graph.task(
        cgride::graph::TaskId::from_string("compile"),
        cgride::graph::TaskKind::Compile);

    task.depends_on(task.id());

    auto errors = cgride::graph::validate_graph_errors(graph);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidArgument &&
          error.message() == "Task cannot depend on itself." &&
          error.detail().has_value() &&
          error.detail().value() == "compile")
      {
        found = true;
      }
    }

    assert(found);
  }

  {
    cgride::graph::Graph graph("cycle");

    auto &prepare = graph.task(
        cgride::graph::TaskId::from_string("prepare"),
        cgride::graph::TaskKind::Prepare);

    auto &compile = graph.task(
        cgride::graph::TaskId::from_string("compile"),
        cgride::graph::TaskKind::Compile);

    auto &link = graph.task(
        cgride::graph::TaskId::from_string("link"),
        cgride::graph::TaskKind::Link);

    prepare.depends_on(link.id());
    compile.depends_on(prepare.id());
    link.depends_on(compile.id());

    auto errors = cgride::graph::validate_graph_errors(graph);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidState &&
          error.message() == "Task dependency cycle detected." &&
          error.detail().has_value())
      {
        found = true;
      }
    }

    assert(found);

    auto result = cgride::graph::topological_sort(graph);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidState ||
           result.error().code() == cgride::core::ErrorCode::InvalidArgument);
  }

  {
    cgride::graph::Graph graph("order");

    auto &prepare = graph.task(
        cgride::graph::TaskId::from_string("prepare"),
        cgride::graph::TaskKind::Prepare);

    auto &compile_core = graph.task(
        cgride::graph::TaskId::from_string("compile:core.cpp"),
        cgride::graph::TaskKind::Compile);

    auto &compile_main = graph.task(
        cgride::graph::TaskId::from_string("compile:main.cpp"),
        cgride::graph::TaskKind::Compile);

    auto &link = graph.task(
        cgride::graph::TaskId::from_string("link:app"),
        cgride::graph::TaskKind::Link);

    compile_core.depends_on(prepare.id());
    compile_main.depends_on(prepare.id());
    link.depends_on({
        compile_core.id(),
        compile_main.id(),
    });

    auto roots = cgride::graph::root_tasks(graph);

    assert(roots.size() == 1);
    assert(roots[0] == &prepare);

    auto leaves = cgride::graph::leaf_tasks(graph);

    assert(leaves.size() == 1);
    assert(leaves[0] == &link);

    auto sorted = cgride::graph::topological_sort(graph);

    assert(sorted);

    const auto &ordered = sorted.value();

    assert(ordered.size() == 4);
    assert(ordered[0] == &prepare);
    assert(ordered[3] == &link);

    const auto compile_core_index =
        ordered[1] == &compile_core ? 1 : 2;

    const auto compile_main_index =
        ordered[1] == &compile_main ? 1 : 2;

    assert(compile_core_index != compile_main_index);
  }

  {
    cgride::graph::Graph graph("multiple-roots-and-leaves");

    auto &prepare = graph.task(
        cgride::graph::TaskId::from_string("prepare"),
        cgride::graph::TaskKind::Prepare);

    auto &generate = graph.task(
        cgride::graph::TaskId::from_string("generate"),
        cgride::graph::TaskKind::Generate);

    auto &compile = graph.task(
        cgride::graph::TaskId::from_string("compile"),
        cgride::graph::TaskKind::Compile);

    compile.depends_on(prepare.id());

    auto roots = cgride::graph::root_tasks(graph);

    assert(roots.size() == 2);
    assert((roots[0] == &prepare || roots[1] == &prepare));
    assert((roots[0] == &generate || roots[1] == &generate));

    auto leaves = cgride::graph::leaf_tasks(graph);

    assert(leaves.size() == 2);
    assert((leaves[0] == &generate || leaves[1] == &generate));
    assert((leaves[0] == &compile || leaves[1] == &compile));
  }

  return 0;
}
