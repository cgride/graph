/**
 *
 *  @file topology.cpp
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
#include <cgride/graph/topology.hpp>

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace cgride::graph
{
  namespace
  {
    using cgride::core::Error;
    using cgride::core::ErrorCode;

    enum class VisitState
    {
      Visiting,
      Visited
    };

    void validate_task_dependencies(const Graph &graph,
                                    const Task &task,
                                    std::vector<Error> &errors)
    {
      for (const auto &dependency : task.dependencies())
      {
        if (!dependency.valid())
        {
          errors.emplace_back(
              ErrorCode::InvalidArgument,
              "Task contains an invalid dependency.",
              std::string(task.id().value()));
          continue;
        }

        if (dependency == task.id())
        {
          errors.emplace_back(
              ErrorCode::InvalidArgument,
              "Task cannot depend on itself.",
              std::string(task.id().value()));
          continue;
        }

        if (!graph.has_task(dependency))
        {
          errors.emplace_back(
              ErrorCode::NotFound,
              "Task dependency refers to an unknown task.",
              std::string(dependency.value()));
        }
      }
    }

    bool visit_for_validation_cycle(
        const Graph &graph,
        const Task &task,
        std::unordered_map<std::string, VisitState> &states,
        std::vector<std::string> &stack,
        std::vector<Error> &errors)
    {
      const auto current = task.id().value();

      auto state = states.find(current);

      if (state != states.end() && state->second == VisitState::Visiting)
      {
        stack.push_back(current);

        std::string cycle;

        for (std::size_t index = 0; index < stack.size(); ++index)
        {
          if (index > 0)
          {
            cycle += " -> ";
          }

          cycle += stack[index];
        }

        errors.emplace_back(
            ErrorCode::InvalidState,
            "Task dependency cycle detected.",
            cycle);

        stack.pop_back();
        return true;
      }

      if (state != states.end() && state->second == VisitState::Visited)
      {
        return false;
      }

      states[current] = VisitState::Visiting;
      stack.push_back(current);

      for (const auto &dependency : task.dependencies())
      {
        const auto *next = graph.find_task(dependency);

        if (next == nullptr)
        {
          continue;
        }

        if (visit_for_validation_cycle(graph, *next, states, stack, errors))
        {
          stack.pop_back();
          states[current] = VisitState::Visited;
          return true;
        }
      }

      stack.pop_back();
      states[current] = VisitState::Visited;

      return false;
    }

    void validate_cycles(const Graph &graph, std::vector<Error> &errors)
    {
      std::unordered_map<std::string, VisitState> states;
      std::vector<std::string> stack;

      for (const auto &task : graph.tasks())
      {
        if (!task || !task->valid())
        {
          continue;
        }

        visit_for_validation_cycle(graph, *task, states, stack, errors);
      }
    }

    void visit_for_topological_sort(
        const Graph &graph,
        const Task &task,
        std::unordered_set<std::string> &visited,
        std::vector<const Task *> &ordered)
    {
      const auto id = task.id().value();

      if (visited.contains(id))
      {
        return;
      }

      visited.insert(id);

      for (const auto &dependency : task.dependencies())
      {
        const auto *next = graph.find_task(dependency);

        if (next != nullptr)
        {
          visit_for_topological_sort(graph, *next, visited, ordered);
        }
      }

      ordered.push_back(&task);
    }

  } // namespace

  std::vector<cgride::core::Error> validate_graph_errors(const Graph &graph)
  {
    std::vector<Error> errors;

    if (graph.empty())
    {
      errors.emplace_back(
          ErrorCode::InvalidArgument,
          "Graph has no tasks.");
      return errors;
    }

    std::unordered_set<std::string> ids;

    for (const auto &task : graph.tasks())
    {
      if (!task)
      {
        errors.emplace_back(
            ErrorCode::InvalidState,
            "Graph contains a null task.");
        continue;
      }

      if (!task->valid())
      {
        errors.emplace_back(
            ErrorCode::InvalidArgument,
            "Graph contains an invalid task.");
        continue;
      }

      if (!ids.insert(task->id().value()).second)
      {
        errors.emplace_back(
            ErrorCode::InvalidArgument,
            "Graph contains duplicate task ids.",
            std::string(task->id().value()));
        continue;
      }

      validate_task_dependencies(graph, *task, errors);
    }

    validate_cycles(graph, errors);

    return errors;
  }

  cgride::core::Result<void> validate_graph(const Graph &graph)
  {
    const auto errors = validate_graph_errors(graph);

    if (!errors.empty())
    {
      return errors.front();
    }

    return cgride::core::Result<void>::ok();
  }

  cgride::core::Result<std::vector<const Task *>> topological_sort(const Graph &graph)
  {
    const auto validation = validate_graph(graph);

    if (!validation)
    {
      return validation.error();
    }

    std::unordered_set<std::string> visited;
    std::vector<const Task *> ordered;

    ordered.reserve(graph.size());

    for (const auto &task : graph.tasks())
    {
      if (!task)
      {
        continue;
      }

      visit_for_topological_sort(graph, *task, visited, ordered);
    }

    return ordered;
  }

  std::vector<const Task *> root_tasks(const Graph &graph)
  {
    std::vector<const Task *> roots;

    for (const auto &task : graph.tasks())
    {
      if (task && task->dependencies().empty())
      {
        roots.push_back(task.get());
      }
    }

    return roots;
  }

  std::vector<const Task *> leaf_tasks(const Graph &graph)
  {
    std::unordered_set<std::string> dependency_ids;

    for (const auto &task : graph.tasks())
    {
      if (!task)
      {
        continue;
      }

      for (const auto &dependency : task->dependencies())
      {
        if (dependency.valid())
        {
          dependency_ids.insert(dependency.value());
        }
      }
    }

    std::vector<const Task *> leaves;

    for (const auto &task : graph.tasks())
    {
      if (!task)
      {
        continue;
      }

      if (!dependency_ids.contains(task->id().value()))
      {
        leaves.push_back(task.get());
      }
    }

    return leaves;
  }

} // namespace cgride::graph
