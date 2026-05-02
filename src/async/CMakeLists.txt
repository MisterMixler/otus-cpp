add_library(async SHARED async.cpp)
set_target_properties(async PROPERTIES OUTPUT_NAME async)

target_include_directories(async
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../bulk/include
)

target_link_libraries(async PRIVATE bulk_processor)
