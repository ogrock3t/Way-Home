cmake_minimum_required(VERSION 3.31)

project(
    labwork6
    VERSION 1.0
    DESCRIPTION "C++ Year 2025 Labwork6"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib)
add_subdirectory(bin)