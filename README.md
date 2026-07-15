# Cgride Graph

Build graph, tasks and dependency ordering for Cgride.

Cgride Graph is the task graph module. It represents build work as logical tasks connected by dependencies. It validates graph structure and returns tasks in dependency-first order.

## Purpose

This module provides the graph-level types used by the Cgride engine:

- task identifiers
- task kinds
- task nodes
- task inputs
- task outputs
- task dependencies
- graph validation
- root task detection
- leaf task detection
- topological sorting

Cgride Graph does not execute tasks, spawn processes, inspect the filesystem, decide whether files are dirty, select compilers or generate build commands.

## Requirements

- C++23
- CMake 3.22 or newer
- Vix.cpp
- Cgride Core

## Build

```bash
vix build --build-target all
```

## Run tests

```bash
vix tests
```

## Release build

```bash
vix build --preset release --build-target all
```

## Install

```bash
vix build --build-target all
sudo cmake --install build-ninja --prefix /usr/local
```

## CMake usage

```cmake
find_package(cgride-graph CONFIG REQUIRED)

target_link_libraries(my_tool PRIVATE cgride::graph)
```

## Basic example

```cpp
#include <cgride/graph/graph.hpp>
#include <cgride/graph/topology.hpp>

int main()
{
  cgride::graph::Graph graph("app");

  auto &prepare = graph.task(
      cgride::graph::TaskId::from_string("prepare"),
      cgride::graph::TaskKind::Prepare);

  auto &compile = graph.task(
      cgride::graph::TaskId::from_string("compile:main.cpp"),
      cgride::graph::TaskKind::Compile);

  auto &link = graph.task(
      cgride::graph::TaskId::from_string("link:app"),
      cgride::graph::TaskKind::Link);

  compile.depends_on(prepare.id());
  link.depends_on(compile.id());

  auto ordered = cgride::graph::topological_sort(graph);

  if (!ordered)
  {
    return 1;
  }

  return 0;
}
```

## Module boundary

Cgride Graph may depend on:

- `cgride::core`

Cgride Graph may be used by:

- `cgride::engine`
- `cgride::executor`
- `cgride::cache`
- `cgride::cli`
- external runtimes
- IDE integrations
- developer tools

Cgride Graph must not depend on:

- `cgride::project`
- `cgride::toolchains`
- `cgride::executor`
- `cgride::cache`
- `cgride::engine`
- `cgride::config`
- `cgride::cli`

## Design rule

This module stores build dependency structure only.

It should answer:

```txt
Which tasks exist?
Which tasks depend on other tasks?
What is the safe dependency-first order?
```

It should not answer:

```txt
Which compiler should be used?
Which command should run?
Which task is dirty?
How many workers should execute tasks?
How should terminal output be printed?
```

Those decisions belong to higher-level Cgride modules.

## License

MIT
