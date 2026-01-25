include_guard(GLOBAL)

include(FetchContent)

function(setup_gtest)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    include(GoogleTest)
    enable_testing()
endfunction()

function(register_gtest_tests target_name)
    gtest_discover_tests(${target_name})
endfunction()
