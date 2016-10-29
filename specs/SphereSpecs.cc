//
// Created by bahuljain on 10/29/16.
//

#include "lib/catch.hpp"
#include "../include/Sphere.h"

TEST_CASE("BoundingBox for Spheres", "[sphere_bbox]") {
    Sphere sphere(0, 0, 0, 10);
    BoundingBox bbox = sphere.getBoundingBox();

    REQUIRE(bbox.x_min == -10);
    REQUIRE(bbox.x_max == 10);
    REQUIRE(bbox.y_min == -10);
    REQUIRE(bbox.y_max == 10);
    REQUIRE(bbox.z_min == -10);
    REQUIRE(bbox.z_max == 10);

    REQUIRE(bbox.center.x == 0);
    REQUIRE(bbox.center.y == 0);
    REQUIRE(bbox.center.z == 0);
}
