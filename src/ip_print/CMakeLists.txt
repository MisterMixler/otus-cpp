add_executable(ip_print main.cpp)
target_include_directories(ip_print PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(ip_print PRIVATE ProjectOptions)
