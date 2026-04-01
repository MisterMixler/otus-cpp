cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cmake/StandardProjectSettings.cmake)

project(editor VERSION ${PROJECT_VERSION})

include(cmake/ProjectOptions.cmake)

add_subdirectory(src/editor)

install(TARGETS editor RUNTIME DESTINATION bin)
set_target_properties(editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
include(cmake/Doxygen.cmake)
add_doxygen_target()
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
