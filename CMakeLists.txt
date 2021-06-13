cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project(ArtStudio)

file(GLOB_RECURSE SOURCE_FILES *.h *.cc)

add_executable(hello ${SOURCE_FILES})
