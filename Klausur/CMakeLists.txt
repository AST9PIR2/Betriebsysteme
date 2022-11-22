cmake_minimum_required(VERSION 3.24)
project(Klausur C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -pthread")

add_executable(Klausur factory.c factory.c)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(Klausur PUBLIC ${LIBRT})
endif()