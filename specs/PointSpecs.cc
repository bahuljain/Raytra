//
// Created by bahuljain on 10/16/16.
//

#include "lib/catch.hpp"
#include "../include/Point.h"

TEST_CASE("Subtraction of two points", "[point_sub]") {
    Point p1, p2;
    Vector v;

    p1 = Point(1, 2, 3);
    p2 = Point(1, 2, 3);
    v = p1.sub(p2);

    REQUIRE(v.i == 0);
    REQUIRE(v.j == 0);
    REQUIRE(v.k == 0);
}

TEST_CASE("Moving a Point along a Vector", "[point_moveAlong]") {
    Point p1, p2;
    Vector v, v2;

    p1 = Point(0, 0, 0);
    v = Vector(15, 46, 10);
    p2 = p1.moveAlong(v);

    REQUIRE(p2.x == 15);
    REQUIRE(p2.y == 46);
    REQUIRE(p2.z == 10);
}

TEST_CASE("Distance-squared between two points", "[point_distance2]") {
    REQUIRE(Point(0, 0, 0).distance2(Point(1, 1, 1)) == 3);
    REQUIRE(Point(0, 10, 3).distance2(Point(1, 1, 1)) == 86);
}