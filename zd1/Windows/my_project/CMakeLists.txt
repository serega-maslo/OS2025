cmake_minimum_required(VERSION 3.10)
project(LibrarySystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/Number)
add_subdirectory(src/Vector)

add_executable(main_app src/main.cpp)

target_link_libraries(main_app
    PRIVATE
        Vector
	Number
)

target_include_directories(main_app
    PRIVATE
        src/Number/include
        src/Vector/include
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CONFIG_POSTFIX "Debug")
else()
    set(CONFIG_POSTFIX "${CMAKE_BUILD_TYPE}")
endif()

if(WIN32)
    add_custom_command(TARGET main_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Vector>
        $<TARGET_FILE_DIR:main_app>/
        COMMENT "Copying Vector DLL to executable directory"
    )
else()
endif()
