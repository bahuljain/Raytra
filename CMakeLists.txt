cmake_minimum_required(VERSION 3.6)
project(Raytra)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cc parse.cc prp.cc Vector.cc Vector.h Camera.cc Camera.h Point.cc Point.h Ray.cc Ray.h)
add_executable(Raytra ${SOURCE_FILES})