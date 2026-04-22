add_library(SameFinderLib STATIC
    src/cli.cpp
    src/same_finder.cpp
)

target_include_directories(SameFinderLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SameFinderLib
    PUBLIC
        ProjectOptions
        Boost::filesystem
        Boost::system
    PRIVATE
        Boost::program_options
)

add_executable(SameFinder main.cpp)
target_link_libraries(SameFinder PRIVATE SameFinderLib)
