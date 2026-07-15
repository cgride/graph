/**
 *
 *  @file task_id.cpp
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
#include <cgride/graph/task_id.hpp>

#include <functional>
#include <utility>

namespace cgride::graph
{
  TaskId::TaskId(std::string value)
      : value_(std::move(value))
  {
  }

  TaskId TaskId::from_string(std::string value)
  {
    return TaskId(std::move(value));
  }

  const std::string &TaskId::value() const noexcept
  {
    return value_;
  }

  bool TaskId::empty() const noexcept
  {
    return value_.empty();
  }

  bool TaskId::valid() const noexcept
  {
    return !value_.empty();
  }

  bool operator==(const TaskId &lhs, const TaskId &rhs) noexcept
  {
    return lhs.value_ == rhs.value_;
  }

  bool operator!=(const TaskId &lhs, const TaskId &rhs) noexcept
  {
    return !(lhs == rhs);
  }

  bool operator<(const TaskId &lhs, const TaskId &rhs) noexcept
  {
    return lhs.value_ < rhs.value_;
  }

  std::string_view to_string(const TaskId &id) noexcept
  {
    return id.value();
  }

  std::size_t TaskIdHash::operator()(const TaskId &id) const noexcept
  {
    return std::hash<std::string>{}(id.value());
  }

} // namespace cgride::graph
