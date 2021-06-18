cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project(ArtStudio)

add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

add_subdirectory(vendor/glfw-3.3.4)

include_directories(
    src # 因不能只从头文件创建库，故此处只要 include 即可
    vendor/glm
    vendor/glfw-3.3.4/include
)

add_subdirectory(example)
