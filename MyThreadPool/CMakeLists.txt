cmake_minimum_required(VERSION 3.0)
project(mythreadpool)

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}) # cmake_current_source_dir
include_directories(${PROJ_DIR})


add_executable(test_threadpool test/test_threadpool.cc threadpool.cc)
target_link_libraries(test_threadpool pthread)