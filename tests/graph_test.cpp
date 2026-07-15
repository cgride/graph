/**
 *
 *  @file graph_test.cpp
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
#include <string>

#include <cgride/graph/graph.hpp>

int main()
{
  {
    cgride::graph::Graph graph;

    assert(graph.name().empty());
    assert(graph.empty());
    assert(graph.size() == 0);
    assert(graph.tasks().empty());

    assert(graph.find_task(cgride::graph::TaskId::from_string("compile")) == nullptr);
    assert(graph.find_task("compile") == nullptr);

    assert(!graph.has_task(cgride::graph::TaskId::from_string("compile")));
    assert(!graph.has_task("compile"));
  }

  {
    cgride::graph::Graph graph("app");

    assert(graph.name() == "app");

    graph.name("server");

    assert(graph.name() == "server");
  }

  {
    cgride::graph::Graph graph("build");

    auto &prepare = graph.task(
        cgride::graph::TaskId::from_string("prepare"),
        cgride::graph::TaskKind::Prepare);

    assert(graph.size() == 1);
    assert(!graph.empty());

    assert(prepare.id() == cgride::graph::TaskId::from_string("prepare"));
    assert(prepare.kind() == cgride::graph::TaskKind::Prepare);

    assert(graph.find_task(cgride::graph::TaskId::from_string("prepare")) == &prepare);
    assert(graph.find_task("prepare") == &prepare);

    assert(graph.has_task(cgride::graph::TaskId::from_string("prepare")));
    assert(graph.has_task("prepare"));
  }

  {
    cgride::graph::Graph graph("named");

    auto &compile = graph.task(
        cgride::graph::TaskId::from_string("compile:main.cpp"),
        cgride::graph::TaskKind::Compile,
        "Compile main.cpp");

    assert(graph.size() == 1);
    assert(compile.name() == "Compile main.cpp");
    assert(compile.kind() == cgride::graph::TaskKind::Compile);
  }

  {
    cgride::graph::Graph graph("manual");

    cgride::graph::Task task(
        cgride::graph::TaskId::from_string("link:app"),
        cgride::graph::TaskKind::Link,
        "Link app");

    task.output("build/app");

    auto &added = graph.add_task(std::move(task));

    assert(graph.size() == 1);
    assert(added.id() == cgride::graph::TaskId::from_string("link:app"));
    assert(added.name() == "Link app");
    assert(added.outputs().size() == 1);
    assert(graph.find_task("link:app") == &added);
  }

  {
    cgride::graph::Graph graph("dependencies");

    auto &compile = graph.task(
        cgride::graph::TaskId::from_string("compile:main.cpp"),
        cgride::graph::TaskKind::Compile);

    auto &link = graph.task(
        cgride::graph::TaskId::from_string("link:app"),
        cgride::graph::TaskKind::Link);

    const auto added = graph.add_dependency(
        link.id(),
        compile.id());

    assert(added);
    assert(link.dependencies().size() == 1);
    assert(link.dependencies()[0] == compile.id());
  }

  {
    cgride::graph::Graph graph("missing-task");

    const auto added = graph.add_dependency(
        cgride::graph::TaskId::from_string("link:app"),
        cgride::graph::TaskId::from_string("compile:main.cpp"));

    assert(!added);
  }

  {
    cgride::graph::Graph graph("stable");

    auto &first = graph.task(
        cgride::graph::TaskId::from_string("first"),
        cgride::graph::TaskKind::Prepare);

    for (int index = 0; index < 32; ++index)
    {
      graph.task(
          cgride::graph::TaskId::from_string("task:" + std::to_string(index)),
          cgride::graph::TaskKind::Custom);
    }

    assert(graph.size() == 33);
    assert(first.id() == cgride::graph::TaskId::from_string("first"));
    assert(graph.find_task("first") == &first);
  }

  {
    cgride::graph::Graph graph("clear");

    graph.task(
        cgride::graph::TaskId::from_string("prepare"),
        cgride::graph::TaskKind::Prepare);

    graph.task(
        cgride::graph::TaskId::from_string("compile"),
        cgride::graph::TaskKind::Compile);

    assert(graph.size() == 2);
    assert(!graph.empty());

    graph.clear();

    assert(graph.empty());
    assert(graph.size() == 0);
    assert(graph.find_task("prepare") == nullptr);
    assert(!graph.has_task("compile"));
  }

  return 0;
}
