cmake_minimum_required(VERSION 3.8)
project(chess_version2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp location_piece_square.h location_piece_square.cpp state.h state.cpp move.h)
add_executable(chess_version2 ${SOURCE_FILES})