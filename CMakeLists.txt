cmake_minimum_required(VERSION 2.6)
project(CMAKEDEMO)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_library(readexp readexp.c)

add_executable(demo demo.c)
target_link_libraries(demo readexp)

enable_testing()
add_test (simple_test ${CMAKE_CURRENT_BINARY_DIR}/demo 1+1)
