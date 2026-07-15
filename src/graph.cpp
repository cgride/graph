/**
 *
 *  @file graph.cpp
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
#include <cgride/graph/graph.hpp>

#include <utility>

namespace cgride::graph
{
  Graph::Graph(std::string name)
      : name_(std::move(name))
  {
  }

  Graph &Graph::name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  Task &Graph::task(TaskId id, TaskKind kind)
  {
    return add_task(Task(std::move(id), kind));
  }

  Task &Graph::task(TaskId id, TaskKind kind, std::string name)
  {
    return add_task(Task(std::move(id), kind, std::move(name)));
  }

  Task &Graph::add_task(Task task)
  {
    auto owned = std::make_unique<Task>(std::move(task));
    auto &reference = *owned;

    tasks_.push_back(std::move(owned));

    return reference;
  }

  bool Graph::add_dependency(const TaskId &id, TaskId dependency)
  {
    auto *task = find_task(id);

    if (task == nullptr)
    {
      return false;
    }

    task->depends_on(std::move(dependency));
    return true;
  }

  Task *Graph::find_task(const TaskId &id) noexcept
  {
    for (auto &task : tasks_)
    {
      if (task && task->id() == id)
      {
        return task.get();
      }
    }

    return nullptr;
  }

  const Task *Graph::find_task(const TaskId &id) const noexcept
  {
    for (const auto &task : tasks_)
    {
      if (task && task->id() == id)
      {
        return task.get();
      }
    }

    return nullptr;
  }

  Task *Graph::find_task(std::string_view id) noexcept
  {
    for (auto &task : tasks_)
    {
      if (task && task->id().value() == id)
      {
        return task.get();
      }
    }

    return nullptr;
  }

  const Task *Graph::find_task(std::string_view id) const noexcept
  {
    for (const auto &task : tasks_)
    {
      if (task && task->id().value() == id)
      {
        return task.get();
      }
    }

    return nullptr;
  }

  bool Graph::has_task(const TaskId &id) const noexcept
  {
    return find_task(id) != nullptr;
  }

  bool Graph::has_task(std::string_view id) const noexcept
  {
    return find_task(id) != nullptr;
  }

  const std::string &Graph::name() const noexcept
  {
    return name_;
  }

  const std::vector<std::unique_ptr<Task>> &Graph::tasks() const noexcept
  {
    return tasks_;
  }

  bool Graph::empty() const noexcept
  {
    return tasks_.empty();
  }

  std::size_t Graph::size() const noexcept
  {
    return tasks_.size();
  }

  void Graph::clear() noexcept
  {
    tasks_.clear();
  }

} // namespace cgride::graph
