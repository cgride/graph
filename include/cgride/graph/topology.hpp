/**
 *
 *  @file topology.hpp
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
#ifndef CGRIDE_GRAPH_TOPOLOGY_HPP
#define CGRIDE_GRAPH_TOPOLOGY_HPP

#include <vector>

#include <cgride/core/error.hpp>
#include <cgride/core/result.hpp>
#include <cgride/graph/graph.hpp>
#include <cgride/graph/task.hpp>
#include <cgride/graph/task_id.hpp>

namespace cgride::graph
{
  /**
   * @brief Validate a task graph and return all detected topology errors.
   *
   * Validation checks the graph model only. It does not execute tasks,
   * inspect input files, inspect output files or decide whether tasks are dirty.
   *
   * @param graph Graph to validate.
   * @return List of topology errors.
   */
  [[nodiscard]] std::vector<cgride::core::Error> validate_graph_errors(const Graph &graph);

  /**
   * @brief Validate a task graph and return success or the first error.
   *
   * @param graph Graph to validate.
   * @return Success when the graph is valid, otherwise first error.
   */
  [[nodiscard]] cgride::core::Result<void> validate_graph(const Graph &graph);

  /**
   * @brief Return tasks in dependency-first order.
   *
   * The returned pointers refer to tasks owned by the input graph. They remain
   * valid as long as the graph is not modified or destroyed.
   *
   * @param graph Graph to order.
   * @return Ordered tasks, or the first topology error.
   */
  [[nodiscard]] cgride::core::Result<std::vector<const Task *>> topological_sort(const Graph &graph);

  /**
   * @brief Return tasks that have no dependencies.
   *
   * @param graph Graph to inspect.
   * @return Tasks with an empty dependency list.
   */
  [[nodiscard]] std::vector<const Task *> root_tasks(const Graph &graph);

  /**
   * @brief Return tasks that are not depended on by any other task.
   *
   * @param graph Graph to inspect.
   * @return Leaf tasks.
   */
  [[nodiscard]] std::vector<const Task *> leaf_tasks(const Graph &graph);

} // namespace cgride::graph

#endif // CGRIDE_GRAPH_TOPOLOGY_HPP
