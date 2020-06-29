cmake_minimum_required(VERSION 3.16)
project(cpplox)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpplox main.cpp lexer.cpp lexer.h token_type.h token.cpp token.h)