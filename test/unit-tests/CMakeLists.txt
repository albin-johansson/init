cmake_minimum_required(VERSION 3.18)

project(init-test-unit-tests CXX)

set(TEST_FILES
    test_main.cpp
    init/ini_section_test.cpp
    init/ini_test.cpp
    init/ini_value_test.cpp
    )

add_executable(UnitTests ${TEST_FILES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(UnitTests PRIVATE
      /EHsc
      /MP
      /W3
      /Zc:__cplusplus  # Force MSVC to use __cplusplus macro with correct value
      /Zc:preprocessor # Enable conforming preprocessor
      /wd5105          # Disable C5105: "macro expansion producing 'defined' has undefined behavior"
      )
endif ()

target_include_directories(UnitTests
    PUBLIC .
    PUBLIC ${INCLUDE_DIR}
    SYSTEM PUBLIC ${gtest_SOURCE_DIR}/include
    SYSTEM PUBLIC ${gtest_SOURCE_DIR})

target_link_libraries(UnitTests PUBLIC gtest)

add_test(NAME UnitTests COMMAND UnitTests)

add_custom_command(
    TARGET UnitTests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RESOURCES_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/resources)