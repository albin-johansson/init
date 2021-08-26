cmake_minimum_required(VERSION 3.18)

project(init-test-unit-tests CXX)

set(TEST_FILES
    test_main.cpp
    )

add_executable(UnitTests ${TEST_FILES})

target_include_directories(UnitTests
    PUBLIC .
    PUBLIC ${INCLUDE_DIR}
    SYSTEM PUBLIC ${gtest_SOURCE_DIR}/include
    SYSTEM PUBLIC ${gtest_SOURCE_DIR})

target_link_libraries(UnitTests PUBLIC gtest)

add_test(NAME UnitTests COMMAND UnitTests)
