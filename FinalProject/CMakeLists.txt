cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME Genes)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_MULTITHREAD       ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)

if(APPLE)
    set(LibArchive_INCLUDE_DIR "/usr/local/opt/libarchive/lib")
endif()

find_package(
        Boost REQUIRED COMPONENTS
            locale
            system
            thread
            filesystem)
find_package(TBB        REQUIRED)
find_package(Threads    REQUIRED)
find_package(LibArchive REQUIRED)

add_executable(
        ${PROJECT_NAME}
            src/main.cpp
            include/config.h
            include/typedefs.h
            src/config.cpp
            src/utils.cpp
            include/utils.h
            include/aho_corasick.h
            include/state.h
            src/aho_corasick.cpp
            src/program.cpp
            include/program.h
            src/reader.cpp
            include/reader.h
            src/matcher.cpp
            include/matcher.h)

target_include_directories(
        ${PROJECT_NAME} PUBLIC
            include
            src)

target_link_libraries(
        ${PROJECT_NAME} PUBLIC
            ${Boost_LIBRARIES}
            ${ICU_LIBRARIES}
            ${LibArchive_LIBRARIES}
            ${TBB_IMPORTED_TARGETS})
