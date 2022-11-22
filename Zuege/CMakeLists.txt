cmake_minimum_required(VERSION 3.24)
project(zuege C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -pthread")

add_executable(zuege main.c)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(zuege PUBLIC ${LIBRT})
endif()
