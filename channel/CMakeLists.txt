cmake_minimum_required(VERSION 3.16)

project(ChannelMatrixMultiplication CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

add_executable(matrix_mult_channel main_std_thread.cpp)

target_include_directories(matrix_mult_channel PUBLIC .)

target_link_libraries(matrix_mult_channel PRIVATE Threads::Threads)