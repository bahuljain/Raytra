cmake_minimum_required(VERSION 3.6)
project(Raytra)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -lIlmThread -std=c++11")

set(SOURCE_FILES main.cc Vector.cc include/Vector.h Camera.cc include/Camera.h Point.cc include/Point.h Ray.cc include/Ray.h include/Surface.h Sphere.cc include/Sphere.h Material.cc include/Material.h include/RGB.h include/parser.h parser.cc)
add_executable(Raytra ${SOURCE_FILES})