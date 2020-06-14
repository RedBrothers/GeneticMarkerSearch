SET(PROJECT_NAME aho_corasick)
project (${PROJECT_NAME})
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
