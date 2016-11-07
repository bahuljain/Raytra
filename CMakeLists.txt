cmake_minimum_required(VERSION 3.6)
project(Raytra)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -lIlmThread -std=c++11")

set(SOURCE_FILES main.cc include/Vector.h Camera.cc include/Camera.h include/Point.h Ray.cc include/Ray.h include/Surface.h Sphere.cc include/Sphere.h include/Material.h include/RGB.h include/Parser.h Parser.cc Triangle.cc include/Triangle.h include/Light.h include/ProgressBar.h Surface.cc BoundingBox.cc include/BoundingBox.h include/BVHNode.h BVHTree.cc include/BVHTree.h)
add_executable(Raytra ${SOURCE_FILES})

file(GLOB TEST_FILES "specs/*.cc")
add_executable(test_out ${TEST_FILES} specs/VectorSpecs.cc specs/PointSpecs.cc specs/RaySpecs.cc specs/BoundingBoxSpecs.cc specs/TriangleSpec.cc specs/SphereSpecs.cc)