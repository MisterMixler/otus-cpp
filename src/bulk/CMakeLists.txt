add_library(bulk_processor STATIC
    command_processor.cpp
)
target_include_directories(bulk_processor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(bulk_processor PUBLIC ProjectOptions)
