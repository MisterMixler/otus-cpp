include_guard(GLOBAL)

add_library(ProjectOptions INTERFACE)

set_target_properties(ProjectOptions PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(ProjectOptions INTERFACE
        /W4
    )
else ()
    target_compile_options(ProjectOptions INTERFACE
        -Wall -Wextra -pedantic -Werror
    )
endif ()
