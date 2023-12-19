cmake_minimum_required (VERSION 2.8)

project (cpp-server)
add_definitions(-Wall -g)


include_directories(../src)

aux_source_directory(./ DIR_SRC)
add_executable(server ${DIR_SRC})

aux_source_directory(./client CLIENT_SRC)
add_executable(client ${CLIENT_SRC} ./event.cpp)

