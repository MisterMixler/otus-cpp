add_library(bulk_processor STATIC
    command_processor.cpp
)
target_include_directories(bulk_processor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(bulk_processor PUBLIC ProjectOptions)

# Объекты .a входят в libasync.so — нужен -fPIC, иначе ошибка линковки shared library.
set_target_properties(bulk_processor PROPERTIES POSITION_INDEPENDENT_CODE ON)
