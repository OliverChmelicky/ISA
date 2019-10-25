cmake_minimum_required(VERSION 3.15)
project(ISA)

set(CMAKE_CXX_STANDARD 14)

add_executable(ISACpp main.cpp methods.cpp socketHelper.cpp parseArgs.cpp dbAccess.cpp )