cmake_minimum_required(VERSION 3.16)
project(ThreadDemo LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_HEADERS common.h)

message(STATUS "Building for Windows")
add_executable(ThreadDemo main_windows.cpp ${COMMON_HEADERS})
target_compile_definitions(ThreadDemo PRIVATE PLATFORM_WINDOWS)

