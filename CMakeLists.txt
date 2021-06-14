cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project(ArtStudio)

include_directories(src) # 因不能只从头文件创建库，故此处只要 include 即可

file(GLOB_RECURSE SOURCE_FILES *.h *.cc)

add_executable(hello ${SOURCE_FILES})
