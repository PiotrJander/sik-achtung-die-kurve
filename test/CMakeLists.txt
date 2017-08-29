cmake_minimum_required(VERSION 3.2)
project(test_runner)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp src/ClientMessage_test.cpp src/GameEvent_test.cpp src/Game_test.cpp src/setup.h
        src/PlayerConnection_test.cpp src/EventBatch_test.cpp src/GameManager_test.cpp src/DynamicBuffer_test.cpp src/ReadBuffer_test.cpp)

add_subdirectory(lib/googletest)

include_directories(../src/)
include_directories(lib/googletest/googletest/include)

add_executable(test_runner ${SOURCE_FILES})
target_link_libraries(test_runner nietacka gtest)

install(TARGETS test_runner DESTINATION bin)

