cmake_minimum_required(VERSION 2.6)
project(CMAKEDEMO)

set(CMAKE_C_FLAGS "-g -Wall")

add_library(readexp readexp.c stack-eval.c)

add_executable(eval-demo eval-demo.c)
target_link_libraries(eval-demo readexp)

add_executable(demo demo.c)
target_link_libraries(demo readexp)
