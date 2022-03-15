cmake_minimum_required(VERSION 3.11)

project(ar)

# Platform setting for raylib
set(PLATFORM "Raspberry Pi" CACHE STRING "Platform")

add_subdirectory(raylib)
add_subdirectory(raylib-cpp)
add_subdirectory(fmt)

find_package(fmt)

add_executable(ar "src/main.cpp" "include/App.h" "src/App.cpp"  "include/MenuApp.h" "src/MenuApp.cpp" "include/CalibrationApp.h" "src/CalibrationApp.cpp" "include/OpticalParams.h" "include/App3D.h" "src/App3D.cpp" "src/Test3DApp.cpp" "include/Test3DApp.h")
target_include_directories(ar PUBLIC "include/")

set_property(TARGET ar PROPERTY CXX_STANDARD 17)

target_link_libraries(ar PUBLIC raylib raylib_cpp fmt::fmt)
