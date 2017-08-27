cmake_minimum_required(VERSION 3.2)
project(achtung)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp)

add_subdirectory(nietacka)
include_directories(include)
include_directories(nietacka)
include_directories(../test/lib/googletest/include)

add_executable(achtung ${SOURCE_FILES})
target_link_libraries(achtung nietacka)

install(TARGETS achtung DESTINATION bin)

