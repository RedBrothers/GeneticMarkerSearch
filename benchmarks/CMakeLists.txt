SET(PROJECT_NAME benchmark)
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} benchmark.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE search aho_corasick)
