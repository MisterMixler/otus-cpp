find_package(Boost REQUIRED)

add_executable(bulk_server
    main.cpp
    output.cpp
    session.cpp
    server.cpp
)

target_include_directories(bulk_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../bulk/include
)

target_link_libraries(bulk_server PRIVATE
    bulk_processor
    Boost::boost
    ProjectOptions
)
