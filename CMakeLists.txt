cmake_minimum_required(VERSION 3.0)
project(graphics)

find_package(glew REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(src)
