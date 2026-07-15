/**
 *
 *  @file task_kind.hpp
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
#ifndef CGRIDE_GRAPH_TASK_KIND_HPP
#define CGRIDE_GRAPH_TASK_KIND_HPP

#include <string_view>

namespace cgride::graph
{
  /**
   * @enum TaskKind
   * @brief Logical kind of a build graph task.
   *
   * TaskKind describes what a node represents inside the graph. It does not
   * contain command execution, compiler selection or toolchain-specific logic.
   */
  enum class TaskKind
  {
    Unknown,
    Prepare,
    Generate,
    Compile,
    Archive,
    Link,
    Copy,
    Custom
  };

  /**
   * @brief Convert a task kind to a stable string.
   *
   * @param kind Task kind.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view to_string(TaskKind kind) noexcept
  {
    switch (kind)
    {
    case TaskKind::Unknown:
      return "Unknown";

    case TaskKind::Prepare:
      return "Prepare";

    case TaskKind::Generate:
      return "Generate";

    case TaskKind::Compile:
      return "Compile";

    case TaskKind::Archive:
      return "Archive";

    case TaskKind::Link:
      return "Link";

    case TaskKind::Copy:
      return "Copy";

    case TaskKind::Custom:
      return "Custom";
    }

    return "Unknown";
  }

  /**
   * @brief Return true if the task kind is known.
   *
   * @param kind Task kind.
   * @return True when the task kind is not Unknown.
   */
  [[nodiscard]] constexpr bool is_known(TaskKind kind) noexcept
  {
    return kind != TaskKind::Unknown;
  }

  /**
   * @brief Return true if the task usually represents a C++ build step.
   *
   * @param kind Task kind.
   * @return True for compile, archive and link tasks.
   */
  [[nodiscard]] constexpr bool is_native_build_step(TaskKind kind) noexcept
  {
    return kind == TaskKind::Compile ||
           kind == TaskKind::Archive ||
           kind == TaskKind::Link;
  }

  /**
   * @brief Return true if the task kind may produce build outputs.
   *
   * @param kind Task kind.
   * @return True when the task kind may produce output files.
   */
  [[nodiscard]] constexpr bool may_produce_outputs(TaskKind kind) noexcept
  {
    return kind == TaskKind::Generate ||
           kind == TaskKind::Compile ||
           kind == TaskKind::Archive ||
           kind == TaskKind::Link ||
           kind == TaskKind::Copy ||
           kind == TaskKind::Custom;
  }

} // namespace cgride::graph

#endif // CGRIDE_GRAPH_TASK_KIND_HPP
