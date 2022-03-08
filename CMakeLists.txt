cmake_minimum_required(VERSION 3.11)

project(ar)

# Platform setting for raylib
set(PLATFORM "Raspberry Pi" CACHE STRING "Platform")

add_subdirectory(raylib)

add_subdirectory(raylib-cpp)

add_executable(ar "src/main.cpp" "include/App.h" "src/App.cpp" "include/constants.h")
target_include_directories(ar PUBLIC "include/")

target_link_libraries(ar PUBLIC raylib raylib_cpp)
