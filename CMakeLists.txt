cmake_minimum_required(VERSION 3.24)
project(biam)

set(CMAKE_CXX_STANDARD 17)

add_executable(biam main.cpp Instance.h StringFunctions.h Instance.cpp StringFunctions.cpp Solver.cpp)
