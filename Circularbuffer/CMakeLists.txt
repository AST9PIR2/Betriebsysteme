cmake_minimum_required(VERSION 3.24)
project(Spielwiese C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -pthread")

add_executable(Spielwiese main.c circularbuffer.c circularbuffer.h)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(Spielwiese PUBLIC ${LIBRT})
endif()