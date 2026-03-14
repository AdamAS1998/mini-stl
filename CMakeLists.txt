cmake_minimum_required(VERSION 3.27)

project(MiniSTL)

set(CMAKE_CXX_STANDARD 20) # c++20 compiler
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header-only library
add_library(ministl INTERFACE)

target_include_directories(ministl INTERFACE include)

# Main executable
add_executable(mini_stl examples/demo.cpp)

target_link_libraries(mini_stl ministl)
