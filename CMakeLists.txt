cmake_minimum_required(VERSION 3.0)
project(graphics)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package(opengl REQUIRED)
find_package(glew REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenCV REQUIRED)

add_definitions(-DGRAPHICS_SHADERS_DIRECTORY="${CMAKE_SOURCE_DIR}/shaders/")

add_subdirectory(src)
