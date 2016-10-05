cmake_minimum_required(VERSION 3.6)
project(Raytra)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cc parse.cc prp.cc Vector.cc include/Vector.h Camera.cc include/Camera.h Point.cc include/Point.h Ray.cc include/Ray.h include/Surface.h surfaces/Sphere.cc include/Sphere.h Material.cc include/Material.h include/RGB.h)
add_executable(Raytra ${SOURCE_FILES})