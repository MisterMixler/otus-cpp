add_library(ip_filter_lib
    ip_address.cpp
    ip_filter.cpp
    ip_reader.cpp
)
target_include_directories(ip_filter_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(ip_filter_lib PRIVATE ProjectOptions)

add_executable(ip_filter main.cpp)
target_link_libraries(ip_filter PRIVATE ip_filter_lib ProjectOptions)
