cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cmake/StandardProjectSettings.cmake)

project(ip_filter VERSION ${PROJECT_VERSION})

include(cmake/ProjectOptions.cmake)

include(cmake/FetchGTest.cmake)
setup_gtest()

add_subdirectory(src/ip_filter)
add_subdirectory(tests)

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
