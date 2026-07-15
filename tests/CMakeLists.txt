function(cgride_graph_add_test name source)
  add_executable(${name}
    ${source}
  )

  target_link_libraries(${name}
    PRIVATE
      cgride::graph
  )

  target_compile_features(${name}
    PRIVATE
      cxx_std_23
  )

  set_target_properties(${name}
    PROPERTIES
      CXX_EXTENSIONS OFF
  )

  add_test(
    NAME ${name}
    COMMAND ${name}
  )
endfunction()

cgride_graph_add_test(cgride_graph_task_id_test task_id_test.cpp)
cgride_graph_add_test(cgride_graph_task_test task_test.cpp)
cgride_graph_add_test(cgride_graph_graph_test graph_test.cpp)
cgride_graph_add_test(cgride_graph_topology_test topology_test.cpp)
