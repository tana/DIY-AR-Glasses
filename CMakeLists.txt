cmake_minimum_required(VERSION 3.11)

project(ar)

# Platform setting for raylib
set(PLATFORM "Raspberry Pi")

add_subdirectory(raylib)

add_subdirectory(raylib-cpp)

add_executable(ar main.cpp)

target_link_libraries(ar PUBLIC raylib raylib_cpp)
