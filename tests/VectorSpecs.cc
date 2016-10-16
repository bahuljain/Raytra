//
// Created by bahuljain on 10/15/16.
//

#include "lib/catch.hpp"
#include "../include/Vector.h"

TEST_CASE("Factorials are computed", "[factorial]") {
    Vector vec1(0, 1, 0);
    Vector vec2(1, 0, 0);

    REQUIRE(Vector(0, 1, 0).dot(Vector(1, 0, 0)) == 0);
    REQUIRE(Vector(0, 1, 0).dot(Vector(0, 0, 1)) == 0);
    REQUIRE(Vector(1, 0, 0).dot(Vector(0, 0, 1)) == 0);
    REQUIRE(Vector(1, 0, 0).dot(Vector(1, 0, 0)) == 1);
    REQUIRE(Vector(1, 2, 3).dot(Vector(1, 2, 3)) == 14);
}
