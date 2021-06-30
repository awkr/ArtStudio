cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project(ArtStudio)

add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

add_subdirectory(vendor/glfw-3.3.4)

add_subdirectory(src)

add_subdirectory(example)
