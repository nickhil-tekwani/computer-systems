cmake_minimum_required(VERSION 3.23)
project(practicum1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(practicum1
        doubly.h
        practicum.c)
