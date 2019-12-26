cmake_minimum_required(VERSION 3.15)
project(source)

set(CMAKE_CXX_STANDARD 14)

add_executable(source main.cpp OP_Table.cpp OP_Table.h PassOne.cpp PassOne.h PassTwo.cpp PassTwo.h Register_Table.cpp Register_Table.h)