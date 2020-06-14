cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME GeneticMarkerSearch)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(libs/aho_corasick)
add_subdirectory(src)
add_subdirectory(benchmarks)
