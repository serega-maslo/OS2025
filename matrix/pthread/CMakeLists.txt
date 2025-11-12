cmake_minimum_required(VERSION 3.16)
project(ThreadDemo LANGUAGES CXX)

# Используем C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Общий заголовок
set(COMMON_HEADERS common.h)

find_package(Threads REQUIRED)
message(STATUS "Building std::thread version")
add_executable(ThreadDemo main_std_thread.cpp ${COMMON_HEADERS})
target_compile_definitions(ThreadDemo PRIVATE PLATFORM_STD_THREAD)

