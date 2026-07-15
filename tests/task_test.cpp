/**
 *
 *  @file task_test.cpp
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
#include <filesystem>
#include <string_view>
#include <vector>

#include <cgride/core/command.hpp>
#include <cgride/graph/task.hpp>

int main()
{
  {
    using cgride::graph::TaskKind;
    using cgride::graph::to_string;

    assert(to_string(TaskKind::Unknown) == std::string_view("Unknown"));
    assert(to_string(TaskKind::Prepare) == std::string_view("Prepare"));
    assert(to_string(TaskKind::Generate) == std::string_view("Generate"));
    assert(to_string(TaskKind::Compile) == std::string_view("Compile"));
    assert(to_string(TaskKind::Archive) == std::string_view("Archive"));
    assert(to_string(TaskKind::Link) == std::string_view("Link"));
    assert(to_string(TaskKind::Copy) == std::string_view("Copy"));
    assert(to_string(TaskKind::Custom) == std::string_view("Custom"));
  }

  {
    using cgride::graph::TaskKind;

    assert(!cgride::graph::is_known(TaskKind::Unknown));
    assert(cgride::graph::is_known(TaskKind::Prepare));
    assert(cgride::graph::is_known(TaskKind::Compile));
    assert(cgride::graph::is_known(TaskKind::Custom));

    assert(cgride::graph::is_native_build_step(TaskKind::Compile));
    assert(cgride::graph::is_native_build_step(TaskKind::Archive));
    assert(cgride::graph::is_native_build_step(TaskKind::Link));
    assert(!cgride::graph::is_native_build_step(TaskKind::Generate));
    assert(!cgride::graph::is_native_build_step(TaskKind::Copy));

    assert(!cgride::graph::may_produce_outputs(TaskKind::Unknown));
    assert(!cgride::graph::may_produce_outputs(TaskKind::Prepare));
    assert(cgride::graph::may_produce_outputs(TaskKind::Generate));
    assert(cgride::graph::may_produce_outputs(TaskKind::Compile));
    assert(cgride::graph::may_produce_outputs(TaskKind::Archive));
    assert(cgride::graph::may_produce_outputs(TaskKind::Link));
    assert(cgride::graph::may_produce_outputs(TaskKind::Copy));
    assert(cgride::graph::may_produce_outputs(TaskKind::Custom));
  }

  {
    cgride::graph::Task task;

    assert(task.id().empty());
    assert(task.kind() == cgride::graph::TaskKind::Unknown);
    assert(task.name().empty());
    assert(task.dependencies().empty());
    assert(task.inputs().empty());
    assert(task.outputs().empty());
    assert(!task.has_command());
    assert(!task.command().has_value());
    assert(!task.valid());
  }

  {
    cgride::graph::Task task(
        cgride::graph::TaskId::from_string("compile:main.cpp"),
        cgride::graph::TaskKind::Compile);

    assert(task.id() == cgride::graph::TaskId::from_string("compile:main.cpp"));
    assert(task.kind() == cgride::graph::TaskKind::Compile);
    assert(task.name().empty());
    assert(task.valid());
  }

  {
    cgride::graph::Task task(
        cgride::graph::TaskId::from_string("link:app"),
        cgride::graph::TaskKind::Link,
        "Link app");

    assert(task.id() == cgride::graph::TaskId::from_string("link:app"));
    assert(task.kind() == cgride::graph::TaskKind::Link);
    assert(task.name() == "Link app");

    task.name("Link executable");

    assert(task.name() == "Link executable");
  }

  {
    cgride::graph::Task task(
        cgride::graph::TaskId::from_string("link:app"),
        cgride::graph::TaskKind::Link);

    task
        .depends_on(cgride::graph::TaskId::from_string("compile:main.cpp"))
        .depends_on(cgride::graph::TaskId::from_string("compile:app.cpp"));

    assert(task.dependencies().size() == 2);
    assert(task.dependencies()[0] == cgride::graph::TaskId::from_string("compile:main.cpp"));
    assert(task.dependencies()[1] == cgride::graph::TaskId::from_string("compile:app.cpp"));
  }

  {
    cgride::graph::Task task(
        cgride::graph::TaskId::from_string("archive:core"),
        cgride::graph::TaskKind::Archive);

    task.depends_on({
        cgride::graph::TaskId::from_string("compile:core.cpp"),
        cgride::graph::TaskId::from_string("compile:utils.cpp"),
    });

    assert(task.dependencies().size() == 2);
    assert(task.dependencies()[0] == cgride::graph::TaskId::from_string("compile:core.cpp"));
    assert(task.dependencies()[1] == cgride::graph::TaskId::from_string("compile:utils.cpp"));
  }

  {
    cgride::graph::Task task(
        cgride::graph::TaskId::from_string("compile:main.cpp"),
        cgride::graph::TaskKind::Compile);

    task
        .input("src/main.cpp")
        .input("include/app.hpp")
        .output("build/main.o");

    assert(task.inputs().size() == 2);
    assert(task.outputs().size() == 1);

    assert(task.inputs()[0] == std::filesystem::path("src/main.cpp"));
    assert(task.inputs()[1] == std::filesystem::path("include/app.hpp"));
    assert(task.outputs()[0] == std::filesystem::path("build/main.o"));
  }

  {
    cgride::graph::Task task(
        cgride::graph::TaskId::from_string("copy:assets"),
        cgride::graph::TaskKind::Copy);

    task.inputs({
        std::filesystem::path("assets/logo.png"),
        std::filesystem::path("assets/style.css"),
    });

    task.outputs({
        std::filesystem::path("build/assets/logo.png"),
        std::filesystem::path("build/assets/style.css"),
    });

    assert(task.inputs().size() == 2);
    assert(task.outputs().size() == 2);

    assert(task.inputs()[0] == std::filesystem::path("assets/logo.png"));
    assert(task.inputs()[1] == std::filesystem::path("assets/style.css"));
    assert(task.outputs()[0] == std::filesystem::path("build/assets/logo.png"));
    assert(task.outputs()[1] == std::filesystem::path("build/assets/style.css"));
  }

  {
    cgride::graph::Task task(
        cgride::graph::TaskId::from_string("custom:generate"),
        cgride::graph::TaskKind::Custom);

    cgride::core::Command command;

    task.command(std::move(command));

    assert(task.has_command());
    assert(task.command().has_value());
  }

  {
    cgride::graph::Task task(
        cgride::graph::TaskId::from_string("invalid"),
        cgride::graph::TaskKind::Unknown);

    assert(!task.valid());
  }

  {
    cgride::graph::Task task(
        cgride::graph::TaskId{},
        cgride::graph::TaskKind::Compile);

    assert(!task.valid());
  }

  return 0;
}
