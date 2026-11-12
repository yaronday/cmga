cmake_minimum_required(VERSION 4.4.5)


add_executable(cmga
    main.cpp
    analyzer/color_grid.cpp
    analyzer/max_color_dfs.cpp
    analyzer/max_color_uf.cpp
    analyzer/union_find.cpp
    cli/cli_utils.cpp
    support/console.cpp
    support/utils.cpp
    support/error_handler.cpp
    io/file_utils.cpp
    io/matfile_handler.cpp
    assets/Resource.rc
)

target_link_libraries(cmga PRIVATE common_lib)

target_include_directories(cmga PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TARGET cmga PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

set_target_properties(cmga PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)