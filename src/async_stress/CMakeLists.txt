add_executable(async_stress stress.cpp)
target_link_libraries(async_stress PRIVATE async)

if (UNIX AND NOT APPLE)
    set_target_properties(async_stress PROPERTIES
        INSTALL_RPATH "\$ORIGIN/../lib"
    )
endif ()
