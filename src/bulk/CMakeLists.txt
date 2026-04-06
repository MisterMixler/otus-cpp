add_library(bulk_lib STATIC
    command_processor.cpp
    console_logger.cpp
    file_logger.cpp
)
target_include_directories(bulk_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(bulk_lib PUBLIC ProjectOptions)

add_executable(bulk main.cpp)
target_link_libraries(bulk PRIVATE bulk_lib)
