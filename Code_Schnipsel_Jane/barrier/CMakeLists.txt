cmake_minimum_required(VERSION 3.16)
project(Code_Schnipsel_Jane C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -pthread")

add_executable(Code_Schnipsel_Jane barrier.c)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(Code_Schnipsel_Jane PUBLIC ${LIBRT})
endif()