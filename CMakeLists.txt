cmake_minimum_required(VERSION 3.18)

project(init
    VERSION 0.1.0
    HOMEPAGE_URL "https://github.com/albin-johansson/init"
    LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/init)

add_subdirectory(test)