cmake_minimum_required(VERSION 3.21)
project(exam_42 C)

set(CMAKE_C_STANDARD 99)

#include_directories(exam_rank_03)
#add_subdirectory(exam_rank_03)

add_executable(exam_42 ./exam_rank_03/min_paint.c)

#target_link_libraries(exam_rank_03)