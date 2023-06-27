cmake_minimum_required(VERSION 3.23)
project(untitled C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -std=c11")

add_executable(untitled main.c main.h load.h move.c load.c display.c eat.c breed.c breed.h eat.h)
