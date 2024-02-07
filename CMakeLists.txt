cmake_minimum_required(VERSION 3.27)
project(Solution)

set(CMAKE_CXX_STANDARD 20)

add_executable(Solution main.cpp
        Token.hpp
        Token.cpp
        Grammar.cpp
        Grammar.hpp)
