add_library(Vector SHARED src/Vector.cpp)

target_compile_definitions(Vector PRIVATE VECTOR_EXPORTS)

target_include_directories(Vector
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../Number/include
)

target_link_libraries(Vector PRIVATE Number)
