cmake_minimum_required(VERSION 3.2)
project(test_runner)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp src/StreamWrappers_test.cpp)

add_subdirectory(lib/googletest)

include_directories(../src/)
include_directories(lib/googletest/googletest/include)

add_executable(test_runner ${SOURCE_FILES})
target_link_libraries(test_runner nietacka gtest)

install(TARGETS test_runner DESTINATION bin)

