add_executable(matrix main.cpp)
target_include_directories(matrix PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(matrix PRIVATE ProjectOptions)
