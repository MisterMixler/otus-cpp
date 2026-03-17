include_guard(GLOBAL)

# Adds a `doxygen` target when Doxygen is available.
#
# Usage (in top-level CMakeLists.txt):
#   set(DOXYGEN_IN  "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
#   set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
#   include(cmake/Doxygen.cmake)
#   add_doxygen_target()
function(add_doxygen_target)
  if(NOT DEFINED DOXYGEN_IN)
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
  endif()
  if(NOT DEFINED DOXYGEN_OUT)
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  endif()

  find_package(Doxygen QUIET)
  if(NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen not found: 'doxygen' target will not be available")
    return()
  endif()

  find_program(DOXYGEN_DOT_EXECUTABLE NAMES dot)
  if(DOXYGEN_DOT_EXECUTABLE)
    set(DOXYGEN_HAVE_DOT YES)
    get_filename_component(DOXYGEN_DOT_PATH "${DOXYGEN_DOT_EXECUTABLE}" DIRECTORY)
  else()
    set(DOXYGEN_HAVE_DOT NO)
    set(DOXYGEN_DOT_PATH "")
  endif()

  configure_file("${DOXYGEN_IN}" "${DOXYGEN_OUT}" @ONLY)

  add_custom_target(doxygen
    COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUT}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
endfunction()

