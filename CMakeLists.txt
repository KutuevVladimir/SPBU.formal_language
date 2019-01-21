cmake_minimum_required(VERSION 3.12)
project(AlgoWithM4ri)

set(CMAKE_CXX_STANDARD 14)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -lm4ri")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(AlgoWithM4ri main.cpp graph.cpp graph.h grammar.cpp grammar.h)
