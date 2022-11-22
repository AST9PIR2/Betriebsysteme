cmake_minimum_required(VERSION 3.16)
project(Aufgabe_6_4 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -pthread")

add_executable(untitled client.c)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(untitled PUBLIC ${LIBRT})
endif()