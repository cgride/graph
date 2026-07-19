cmake_minimum_required(VERSION 3.22)

project(
  cgride-graph
  VERSION 0.1.1
  DESCRIPTION "Build graph, tasks and dependency ordering for Cgride."
  HOMEPAGE_URL "https://github.com/cgride/graph"
  LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(CGRIDE_GRAPH_BUILD_TESTS "Build Cgride graph tests" ON)
option(CGRIDE_GRAPH_INSTALL "Install Cgride graph" ON)

if(NOT TARGET cgride::core)
  find_package(cgride-core CONFIG REQUIRED)
endif()

add_library(cgride_graph
  src/task_id.cpp
  src/task.cpp
  src/graph.cpp
  src/topology.cpp
)

add_library(cgride::graph ALIAS cgride_graph)

target_compile_features(cgride_graph
  PUBLIC
    cxx_std_23
)

target_include_directories(cgride_graph
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(cgride_graph
  PUBLIC
    cgride::core
)

set_target_properties(cgride_graph
  PROPERTIES
    OUTPUT_NAME cgride_graph
    EXPORT_NAME graph
    CXX_EXTENSIONS OFF
)

if(CGRIDE_GRAPH_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(CGRIDE_GRAPH_INSTALL)
  install(
    TARGETS cgride_graph
    EXPORT cgride-graph-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(
    EXPORT cgride-graph-targets
    NAMESPACE cgride::
    FILE cgride-graph-targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-graph
  )

  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cgride-graph-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cgride-graph-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-graph
  )

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cgride-graph-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/cgride-graph-config.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/cgride-graph-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-graph
  )
endif()
