cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cmake/StandardProjectSettings.cmake)

project(stl_allocator VERSION ${PROJECT_VERSION})

include(cmake/ProjectOptions.cmake)

include(CTest)
enable_testing()

include(cmake/FetchGTest.cmake)
setup_gtest()

add_subdirectory(src/stl_allocator)
add_subdirectory(tests)

install(TARGETS stl_allocator RUNTIME DESTINATION bin)
set_target_properties(stl_allocator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
