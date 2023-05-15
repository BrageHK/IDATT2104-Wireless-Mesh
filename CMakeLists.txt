cmake_minimum_required(VERSION 3.24)
project(Mesh)

set(CMAKE_CXX_STANDARD 17)

add_executable(Mesh main.cpp node/Node.cpp node/Node.h worker/Workers.cpp worker/Workers.h)
