cmake_minimum_required(VERSION 3.2)
project(nietacka)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES ClientMessage.cpp ClientMessage.h MockSocket.cpp MockSocket.h StreamUtils.h Client.cpp Client.h Server.cpp Server.h GameEvent.h GameEvent.cpp crc32c.cpp crc32c.h PixelEvent.h PixelEvent.cpp)

add_library(nietacka SHARED STATIC ${SOURCE_FILES})
