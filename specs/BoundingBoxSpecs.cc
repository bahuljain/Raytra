//
// Created by bahuljain on 10/29/16.
//

#include "lib/catch.hpp"
#include "../include/BoundingBox.h"

TEST_CASE("Intersection of Ray with BoundingBox", "[bounding_box_intersects]") {
    BoundingBox bb(0, 10, 0, 10, 0, 10);
    Ray ray(Point(-10, -10, -10), Vector(1, 0, 0));

    REQUIRE(!bb.intersects(ray));

    ray = Ray(Point(-10, -10, -10), Vector(1, 1, 1).norm());

    REQUIRE(bb.intersects(ray));

    ray = Ray(Point(20, 20, 20), Vector(-1, -1, -1).norm());

    REQUIRE(bb.intersects(ray));

    ray = Ray(Point(-5, 5, 5), Vector(1, 0, 0).norm());

    REQUIRE(bb.intersects(ray));
}