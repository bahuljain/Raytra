//
// Created by bahuljain on 10/16/16.
//

#include "lib/catch.hpp"
#include "../include/Ray.h"

TEST_CASE("Getting a point on a Ray", "[ray_getPointOnIt]") {
    Ray ray(Point(0, 0, 0), Vector(1, 0, 0));
    Point p = ray.getPointOnIt(10);

    REQUIRE(p.x == 10);
    REQUIRE(p.y == 0);
    REQUIRE(p.z == 0);

    Ray ray2(Point(0, 0, 0), Vector(1, 1, 1));
    Point p2 = ray2.getPointOnIt(-5);

    REQUIRE(p2.x == -5);
    REQUIRE(p2.y == -5);
    REQUIRE(p2.z == -5);
}

TEST_CASE("Getting the offset of a Point on the ray from the origin",
          "[ray_getOffsetFromOrigin]") {
    Ray ray(Point(0, 0, 0), Vector(0, 1, 0));
    Point p(0, 10, 0);

    REQUIRE(ray.getOffsetFromOrigin(p) == 10);

    Ray ray2(Point(0, 0, 0), Vector(1, 1, 1));
    Point p2(-5, -5, -5);

    REQUIRE(ray2.getOffsetFromOrigin(p2) == -5);
}
