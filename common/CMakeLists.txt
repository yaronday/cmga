cmake_minimum_required(VERSION 4.4.5)

add_library(common_lib INTERFACE)

target_include_directories(common_lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)