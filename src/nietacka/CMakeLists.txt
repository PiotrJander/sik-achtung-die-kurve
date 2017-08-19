cmake_minimum_required(VERSION 3.2)
project(nietacka)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES ClientMessage.cpp ClientMessage.h MockSocket.cpp MockSocket.h InputStreamWrapper.cpp InputStreamWrapper.h OutputStreamWrapper.cpp OutputStreamWrapper.h Client.cpp Client.h Server.cpp Server.h)

add_library(nietacka SHARED STATIC ${SOURCE_FILES})
