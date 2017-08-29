cmake_minimum_required(VERSION 3.2)
project(achtung)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")

# -DELPP_DISABLE_INFO_LOGS

set(SOURCE_FILES server.cpp client.cpp)

add_subdirectory(nietacka)
include_directories(include)
include_directories(nietacka)
include_directories(../test/lib/googletest/include)

#add_executable(achtung ${SOURCE_FILES})
add_executable(siktacka-server server.cpp)
add_executable(siktacka-client client.cpp)
add_executable(aux-client aux-client.cpp)
target_link_libraries(siktacka-server nietacka)
target_link_libraries(siktacka-client nietacka)
target_link_libraries(aux-client nietacka)

install(TARGETS siktacka-server siktacka-client aux-client DESTINATION .)

