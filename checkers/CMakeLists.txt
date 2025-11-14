cmake_minimum_required(VERSION 3.29)
project(checkers C)

set(CMAKE_C_STANDARD 11)

add_executable(checkers main.c
        dama.h
        dama.c)
