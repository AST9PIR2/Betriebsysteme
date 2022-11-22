cmake_minimum_required(VERSION 3.16)
project(untitled C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -pthread")

add_executable(untitled printserver.c)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(untitled PUBLIC ${LIBRT})
endif()