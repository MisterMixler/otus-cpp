add_executable(editor main.cpp)
target_include_directories(editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(editor PRIVATE ProjectOptions)
