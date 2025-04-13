cmake_minimum_required(VERSION 3.20)

project(8chip)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(8CHIP_BUILD_TESTS OFF CACHE BOOL "Whether to build unit tests")

add_executable(8chip_main)
set_target_properties(8chip_main PROPERTIES OUTPUT_NAME "8chip")
set_target_properties(8chip_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
target_include_directories(8chip_main PRIVATE src)

add_subdirectory(src)

