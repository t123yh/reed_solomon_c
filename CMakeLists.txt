cmake_minimum_required(VERSION 3.12)
project(reed_solomon_c)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu89")


add_executable(reed_solomon_c main.cpp reedsolomon.h reedsolomon.c matrixoperations.c matrixoperations.h galoisconstants.h common.c common.h galois.c galois.h radix-tree.c radix-tree.h)
