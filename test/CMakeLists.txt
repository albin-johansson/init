cmake_minimum_required(VERSION 3.18)

project(init-test CXX)

enable_testing()

include(DownloadProject)

download_project(PROJ googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    "UPDATE_DISCONNECTED 1")

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
add_subdirectory(unit-tests)