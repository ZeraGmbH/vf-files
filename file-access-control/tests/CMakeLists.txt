add_test(NAME    file-access-control-unittest
         COMMAND file-access-control-unittest)

file(GLOB UNITTESTS
    gtest/unittest-*.cpp
)

add_executable(file-access-control-unittest
    ${UNITTESTS}
)

target_include_directories(file-access-control-unittest
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    ${PROJECT_SOURCE_DIR}/src
    )

target_link_libraries(file-access-control-unittest
    PUBLIC
    gtest
    gtest_main
    file-access-control-lib
)
