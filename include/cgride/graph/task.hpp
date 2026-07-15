/**
 *
 *  @file task.hpp
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
#ifndef CGRIDE_GRAPH_TASK_HPP
#define CGRIDE_GRAPH_TASK_HPP

#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <cgride/core/command.hpp>
#include <cgride/graph/task_id.hpp>
#include <cgride/graph/task_kind.hpp>

namespace cgride::graph
{
  /**
   * @class Task
   * @brief One logical node in a build graph.
   *
   * Task stores graph-level build intent. It may carry a command, but it does
   * not execute the command, manage processes or decide scheduling.
   */
  class Task
  {
  public:
    /**
     * @brief Construct an empty task.
     */
    Task() = default;

    /**
     * @brief Construct a task.
     *
     * @param id Task identifier.
     * @param kind Task kind.
     */
    Task(TaskId id, TaskKind kind);

    /**
     * @brief Construct a named task.
     *
     * @param id Task identifier.
     * @param kind Task kind.
     * @param name Human-readable task name.
     */
    Task(TaskId id, TaskKind kind, std::string name);

    /**
     * @brief Set the human-readable task name.
     *
     * @param name Task name.
     * @return Reference to this task.
     */
    Task &name(std::string name);

    /**
     * @brief Add one dependency.
     *
     * @param id Dependency task id.
     * @return Reference to this task.
     */
    Task &depends_on(TaskId id);

    /**
     * @brief Add many dependencies.
     *
     * @param ids Dependency task ids.
     * @return Reference to this task.
     */
    Task &depends_on(std::vector<TaskId> ids);

    /**
     * @brief Add one input path.
     *
     * @param path Input path.
     * @return Reference to this task.
     */
    Task &input(std::filesystem::path path);

    /**
     * @brief Add many input paths.
     *
     * @param paths Input paths.
     * @return Reference to this task.
     */
    Task &inputs(std::vector<std::filesystem::path> paths);

    /**
     * @brief Add one output path.
     *
     * @param path Output path.
     * @return Reference to this task.
     */
    Task &output(std::filesystem::path path);

    /**
     * @brief Add many output paths.
     *
     * @param paths Output paths.
     * @return Reference to this task.
     */
    Task &outputs(std::vector<std::filesystem::path> paths);

    /**
     * @brief Set the command carried by this task.
     *
     * @param command Command data.
     * @return Reference to this task.
     */
    Task &command(cgride::core::Command command);

    /**
     * @brief Access the task id.
     */
    [[nodiscard]] const TaskId &id() const noexcept;

    /**
     * @brief Access the task kind.
     */
    [[nodiscard]] TaskKind kind() const noexcept;

    /**
     * @brief Access the human-readable task name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Access dependency task ids.
     */
    [[nodiscard]] const std::vector<TaskId> &dependencies() const noexcept;

    /**
     * @brief Access input paths.
     */
    [[nodiscard]] const std::vector<std::filesystem::path> &inputs() const noexcept;

    /**
     * @brief Access output paths.
     */
    [[nodiscard]] const std::vector<std::filesystem::path> &outputs() const noexcept;

    /**
     * @brief Access the optional task command.
     */
    [[nodiscard]] const std::optional<cgride::core::Command> &command() const noexcept;

    /**
     * @brief Return true if the task carries a command.
     */
    [[nodiscard]] bool has_command() const noexcept;

    /**
     * @brief Return true when the task has a valid id and known kind.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    TaskId id_{};
    TaskKind kind_{TaskKind::Unknown};
    std::string name_{};
    std::vector<TaskId> dependencies_{};
    std::vector<std::filesystem::path> inputs_{};
    std::vector<std::filesystem::path> outputs_{};
    std::optional<cgride::core::Command> command_{};
  };

} // namespace cgride::graph

#endif // CGRIDE_GRAPH_TASK_HPP
