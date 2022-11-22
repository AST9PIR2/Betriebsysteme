cmake_minimum_required(VERSION 3.24)
project(Aufgabe_3 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Aufgabe_3
        Aufgabe_3_1/matrix.c)
