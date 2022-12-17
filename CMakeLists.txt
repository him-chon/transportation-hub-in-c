cmake_minimum_required(VERSION 3.23)
project(comp9024_logistics C)

set(CMAKE_C_STANDARD 11)

add_executable(comp9024_logistics logistics.c WGraph.c)
