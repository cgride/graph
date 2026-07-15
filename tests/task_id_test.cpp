/**
 *
 *  @file task_id_test.cpp
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
#include <string_view>
#include <unordered_set>

#include <cgride/graph/task_id.hpp>

int main()
{
  {
    cgride::graph::TaskId id;

    assert(id.value().empty());
    assert(id.empty());
    assert(!id.valid());
    assert(cgride::graph::to_string(id) == std::string_view(""));
  }

  {
    cgride::graph::TaskId id("compile:main.cpp");

    assert(id.value() == "compile:main.cpp");
    assert(!id.empty());
    assert(id.valid());
    assert(cgride::graph::to_string(id) == std::string_view("compile:main.cpp"));
  }

  {
    auto id = cgride::graph::TaskId::from_string("link:app");

    assert(id.value() == "link:app");
    assert(id.valid());
  }

  {
    auto first = cgride::graph::TaskId::from_string("compile:main.cpp");
    auto second = cgride::graph::TaskId::from_string("compile:main.cpp");
    auto third = cgride::graph::TaskId::from_string("link:app");

    assert(first == second);
    assert(first != third);
    assert(first < third);
  }

  {
    std::unordered_set<cgride::graph::TaskId, cgride::graph::TaskIdHash> ids;

    ids.insert(cgride::graph::TaskId::from_string("prepare"));
    ids.insert(cgride::graph::TaskId::from_string("compile"));
    ids.insert(cgride::graph::TaskId::from_string("compile"));

    assert(ids.size() == 2);
    assert(ids.contains(cgride::graph::TaskId::from_string("prepare")));
    assert(ids.contains(cgride::graph::TaskId::from_string("compile")));
    assert(!ids.contains(cgride::graph::TaskId::from_string("link")));
  }

  {
    cgride::graph::TaskIdHash hash;

    auto first = hash(cgride::graph::TaskId::from_string("compile"));
    auto second = hash(cgride::graph::TaskId::from_string("compile"));

    assert(first == second);
  }

  return 0;
}
