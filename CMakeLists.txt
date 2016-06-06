cmake_minimum_required(VERSION 3.3)
project(parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c token.h symdef.h scanner.c parser.h parser.c scanner.h)
add_executable(parser ${SOURCE_FILES})