add_library(Number STATIC src/Number.cpp)

set_target_properties(Number PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(Number
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
