cmake_minimum_required(VERSION 2.8)
project(sam)

add_definitions("-std=c++11")

add_subdirectory(tests)
add_subdirectory(samples)
add_subdirectory(sasm)
