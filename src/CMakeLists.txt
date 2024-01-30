cmake_minimum_required (VERSION 2.8)

project (cpp-server)
add_definitions(-Wall -g -pthread)



aux_source_directory(./lib DIR_SRC)
add_executable(server ${DIR_SRC} server.cpp)
target_link_libraries(server PRIVATE pthread)

aux_source_directory(./client CLIENT_SRC)
add_executable(client ${DIR_SRC} client.cpp)
target_link_libraries(client PRIVATE pthread)
