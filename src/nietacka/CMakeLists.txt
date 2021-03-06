cmake_minimum_required(VERSION 3.2)
project(nietacka)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES ClientMessage.cpp ClientMessage.h event/GameEvent.h event/GameEvent.cpp
        crc32c.cpp crc32c.h event/PixelEvent.h event/PixelEvent.cpp event/PlayerEliminatedEvent.cpp
        event/PlayerEliminatedEvent.h event/GameOverEvent.h event/GameOverEvent.cpp event/NewGameEvent.cpp
        event/NewGameEvent.h Game.cpp Game.h PlayerConnection.h Random.h IUdpWorker.h IDatagram.h PlayerConnection.cpp
        IDatagramObserver.h EventBatch.cpp EventBatch.h GameManager.h GameManager.cpp UdpWorker.cpp UdpWorker.h
        Exceptions.h easylogging++.cc easylogging++.h Socket.cpp Socket.h Datagram.h DynamicBuffer.cpp DynamicBuffer.h
        Client.cpp Client.h ReadBuffer.cpp ReadBuffer.h)

include_directories(../../test/lib/googletest/include/)

add_library(nietacka SHARED STATIC ${SOURCE_FILES})
