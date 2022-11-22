cmake_minimum_required(VERSION 3.24)
project(Vorjahresklausur C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -pthread")

add_executable(Vorjahresklausur main.c)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(Vorjahresklausur PUBLIC ${LIBRT})
endif()
