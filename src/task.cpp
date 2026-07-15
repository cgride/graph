/**
 *
 *  @file task.cpp
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
#include <cgride/graph/task.hpp>

namespace cgride::graph
{
  Task::Task(TaskId id, TaskKind kind)
      : id_(std::move(id)),
        kind_(kind)
  {
  }

  Task::Task(TaskId id, TaskKind kind, std::string name)
      : id_(std::move(id)),
        kind_(kind),
        name_(std::move(name))
  {
  }

  Task &Task::name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  Task &Task::depends_on(TaskId id)
  {
    dependencies_.push_back(std::move(id));
    return *this;
  }

  Task &Task::depends_on(std::vector<TaskId> ids)
  {
    for (auto &id : ids)
    {
      dependencies_.push_back(std::move(id));
    }

    return *this;
  }

  Task &Task::input(std::filesystem::path path)
  {
    inputs_.push_back(std::move(path));
    return *this;
  }

  Task &Task::inputs(std::vector<std::filesystem::path> paths)
  {
    for (auto &path : paths)
    {
      inputs_.push_back(std::move(path));
    }

    return *this;
  }

  Task &Task::output(std::filesystem::path path)
  {
    outputs_.push_back(std::move(path));
    return *this;
  }

  Task &Task::outputs(std::vector<std::filesystem::path> paths)
  {
    for (auto &path : paths)
    {
      outputs_.push_back(std::move(path));
    }

    return *this;
  }

  Task &Task::command(cgride::core::Command command)
  {
    command_ = std::move(command);
    return *this;
  }

  const TaskId &Task::id() const noexcept
  {
    return id_;
  }

  TaskKind Task::kind() const noexcept
  {
    return kind_;
  }

  const std::string &Task::name() const noexcept
  {
    return name_;
  }

  const std::vector<TaskId> &Task::dependencies() const noexcept
  {
    return dependencies_;
  }

  const std::vector<std::filesystem::path> &Task::inputs() const noexcept
  {
    return inputs_;
  }

  const std::vector<std::filesystem::path> &Task::outputs() const noexcept
  {
    return outputs_;
  }

  const std::optional<cgride::core::Command> &Task::command() const noexcept
  {
    return command_;
  }

  bool Task::has_command() const noexcept
  {
    return command_.has_value();
  }

  bool Task::valid() const noexcept
  {
    return id_.valid() && is_known(kind_);
  }

} // namespace cgride::graph
