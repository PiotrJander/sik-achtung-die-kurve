cmake_minimum_required(VERSION 3.2)
project(nietacka)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES ClientMessage.cpp ClientMessage.h MockSocket.cpp MockSocket.h InputStreamUtils.h OutputStreamUtils.h Client.cpp Client.h Server.cpp Server.h Event_NewGame.cpp Event_NewGame.h Event_Pixel.cpp Event_Pixel.h Event_PlayerEliminated.cpp Event_PlayerEliminated.h Event_GameOver.cpp Event_GameOver.h)

add_library(nietacka SHARED STATIC ${SOURCE_FILES})
