cmake_minimum_required (VERSION 2.8)

project(client)

include_directories(../../)

add_executable(client client.cpp ../event.cpp)
