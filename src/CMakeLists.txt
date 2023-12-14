cmake_minimum_required (VERSION 2.8)

project (cpp-server)
add_definitions(-Wall -g)
aux_source_directory(./ DIR_SRC)
add_executable(server ${DIR_SRC})


