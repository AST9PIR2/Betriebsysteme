cmake_minimum_required(VERSION 3.24)
project(Aufgabe_4_1 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Aufgabe_4_1 consumerproducer.c)
