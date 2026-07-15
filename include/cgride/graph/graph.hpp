/**
 *
 *  @file graph.hpp
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
#ifndef CGRIDE_GRAPH_GRAPH_HPP
#define CGRIDE_GRAPH_GRAPH_HPP

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <cgride/graph/task.hpp>
#include <cgride/graph/task_id.hpp>
#include <cgride/graph/task_kind.hpp>

namespace cgride::graph
{
  /**
   * @class Graph
   * @brief Directed task graph for build planning.
   *
   * Graph owns logical build tasks and their dependency relationships.
   * It does not execute tasks, schedule processes, inspect the filesystem
   * or decide whether a task is dirty.
   */
  class Graph
  {
  public:
    /**
     * @brief Construct an empty graph.
     */
    Graph() = default;

    /**
     * @brief Construct a named graph.
     *
     * @param name Human-readable graph name.
     */
    explicit Graph(std::string name);

    Graph(const Graph &) = delete;
    Graph &operator=(const Graph &) = delete;

    Graph(Graph &&) noexcept = default;
    Graph &operator=(Graph &&) noexcept = default;

    /**
     * @brief Set the human-readable graph name.
     *
     * @param name Graph name.
     * @return Reference to this graph.
     */
    Graph &name(std::string name);

    /**
     * @brief Create a task and add it to the graph.
     *
     * @param id Task id.
     * @param kind Task kind.
     * @return Reference to the created task.
     */
    Task &task(TaskId id, TaskKind kind);

    /**
     * @brief Create a named task and add it to the graph.
     *
     * @param id Task id.
     * @param kind Task kind.
     * @param name Human-readable task name.
     * @return Reference to the created task.
     */
    Task &task(TaskId id, TaskKind kind, std::string name);

    /**
     * @brief Add an existing task to the graph.
     *
     * @param task Task to add.
     * @return Reference to the added task.
     */
    Task &add_task(Task task);

    /**
     * @brief Add a dependency between two existing or future tasks.
     *
     * The dependency is stored on the task identified by id. Validation and
     * topology are responsible for reporting missing dependency targets.
     *
     * @param id Task that depends on another task.
     * @param dependency Dependency task id.
     * @return True when the task was found and the dependency was added.
     */
    bool add_dependency(const TaskId &id, TaskId dependency);

    /**
     * @brief Find a task by id.
     *
     * @param id Task id.
     * @return Pointer to the task or nullptr.
     */
    [[nodiscard]] Task *find_task(const TaskId &id) noexcept;

    /**
     * @brief Find a task by id.
     *
     * @param id Task id.
     * @return Pointer to the task or nullptr.
     */
    [[nodiscard]] const Task *find_task(const TaskId &id) const noexcept;

    /**
     * @brief Find a task by string id.
     *
     * @param id Task id string.
     * @return Pointer to the task or nullptr.
     */
    [[nodiscard]] Task *find_task(std::string_view id) noexcept;

    /**
     * @brief Find a task by string id.
     *
     * @param id Task id string.
     * @return Pointer to the task or nullptr.
     */
    [[nodiscard]] const Task *find_task(std::string_view id) const noexcept;

    /**
     * @brief Return true if a task with this id exists.
     *
     * @param id Task id.
     * @return True when the graph contains the task.
     */
    [[nodiscard]] bool has_task(const TaskId &id) const noexcept;

    /**
     * @brief Return true if a task with this string id exists.
     *
     * @param id Task id string.
     * @return True when the graph contains the task.
     */
    [[nodiscard]] bool has_task(std::string_view id) const noexcept;

    /**
     * @brief Access the graph name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Access all tasks.
     */
    [[nodiscard]] const std::vector<std::unique_ptr<Task>> &tasks() const noexcept;

    /**
     * @brief Return true if the graph has no tasks.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Return the number of tasks.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Remove all tasks.
     */
    void clear() noexcept;

  private:
    std::string name_{};
    std::vector<std::unique_ptr<Task>> tasks_{};
  };

} // namespace cgride::graph

#endif // CGRIDE_GRAPH_GRAPH_HPP
