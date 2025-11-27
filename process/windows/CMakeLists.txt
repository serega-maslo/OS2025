cmake_minimum_required(VERSION 3.10)

project(ProcessKiller LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(KILLER_SOURCES Killer.cpp)

set(USER_SOURCES User.cpp)

add_executable(Killer ${KILLER_SOURCES})

add_executable(User ${USER_SOURCES})

target_link_libraries(Killer kernel32)
target_link_libraries(User kernel32)