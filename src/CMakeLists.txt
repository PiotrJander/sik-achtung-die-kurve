cmake_minimum_required(VERSION 3.2)
project(achtung)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES server.cpp client.cpp)

add_subdirectory(nietacka)
include_directories(include)
include_directories(nietacka)
include_directories(../test/lib/googletest/include)

#add_executable(achtung ${SOURCE_FILES})
add_executable(server server.cpp)
add_executable(client client.cpp)
target_link_libraries(server nietacka)
target_link_libraries(client nietacka)

install(TARGETS server client DESTINATION bin)

