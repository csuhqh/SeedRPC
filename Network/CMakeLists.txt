cmake_minimum_required(VERSION 3.0)
project("IO_Network")

include_directories(CMAKE_CURRENT_SOURCE_DIR)

add_executable(test_client test/test_client.cpp 00_client.cc)
add_executable(test_block_io test/test_block_io.cpp 01_bio.cc)
add_executable(test_nonblock_io test/test_nonblock_io.cpp 02_nonbio.cc)
add_executable(test_select test/test_multiplexing_select.cpp 03_select.cc)
add_executable(test_epoll test/test_multiplexing_epoll.cpp 04_epoll.cc)