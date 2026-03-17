add_library(stl_allocator_lib
    stl_allocator.cpp
)
target_include_directories(stl_allocator_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(stl_allocator_lib PRIVATE ProjectOptions)

add_executable(stl_allocator main.cpp)
target_link_libraries(stl_allocator PRIVATE stl_allocator_lib ProjectOptions)
