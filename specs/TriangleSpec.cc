//
// Created by bahuljain on 10/29/16.
//

#include "lib/catch.hpp"
#include "../include/Triangle.h"

TEST_CASE("Triangle Normal", "[triangle_normal]") {
    Triangle triangle (0, 0, 0, 10, 0, 0, 5, 5, 0);

    REQUIRE(triangle.getSurfaceNormal(Point()).dot(Vector(0, 0, 1)) == 1);
}

TEST_CASE("BoundingBox for Triangles", "[triangle_bbox]") {
    Triangle triangle (0, 0, 0, 10, 0, 0, 5, 5, 0);
    BoundingBox bbox = triangle.getBoundingBox();

    REQUIRE(bbox.center.x == 5);
    REQUIRE(bbox.center.y == 2.5);
    REQUIRE(bbox.center.z == 0);

    triangle = Triangle(0, 0, 0, 10, 5, -7, 2, 20, 2);
    bbox = triangle.getBoundingBox();

    REQUIRE(bbox.center.x == 5);
    REQUIRE(bbox.center.y == 10);
    REQUIRE(bbox.center.z == -2.5);
}