cmake_minimum_required(VERSION 3.10)

project(MatrixMult_Pthread CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES
    main_pthread.cpp
)

add_executable(pthread_demo ${SOURCE_FILES})

target_compile_definitions(pthread_demo PUBLIC NOMINMAX)

find_package(Threads REQUIRED)

target_link_libraries(pthread_demo Threads::Threads)