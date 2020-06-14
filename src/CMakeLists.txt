SET(APP_NAME run_search)
set(LIB_NAME search)

set(Boost_USE_MULTITHREAD       ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(TBB        REQUIRED)
find_package(Threads    REQUIRED)
find_package(Boost      REQUIRED COMPONENTS
        locale
        iostreams
        system
        thread
        filesystem)

add_library(${LIB_NAME} STATIC
        program.hpp
        program.cpp
        matcher.hpp
        matcher.cpp
        reader.hpp
        reader.cpp
        config.hpp
        config.cpp
        utils.hpp
        utils.cpp)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIB_NAME} PUBLIC
        aho_corasick
        ${Boost_LIBRARIES}
        Threads::Threads
        TBB::tbb)

add_executable(${APP_NAME} main.cpp)
target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME})
