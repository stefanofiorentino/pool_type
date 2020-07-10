cmake_minimum_required(VERSION 3.16)
project(pool_type)

set(CMAKE_CXX_STANDARD 17)

include(cmake/BuildDefaults.cmake)

if(BUILD_TESTING)
    fetch_content_googletest()

    enable_testing()
    add_subdirectory("test")
endif()
