//
// Created by bahuljain on 10/15/16.
//

#include "lib/catch.hpp"
#include "../include/Vector.h"

TEST_CASE("Magnitude of a Vector", "[vector_magnitude]") {
    REQUIRE(Vector(0, 1, 0).mag() == 1);
    REQUIRE(Vector(3, 4, 12).mag() == 13);
    REQUIRE(Vector(3, 4, -3).mag() == sqrtf(34));
}

TEST_CASE("Normalized form of a Vector", "[vector_normalized]") {
    Vector vec, vec2;

    vec = Vector(0, 1, 0);
    vec2 = vec.norm();

    REQUIRE(vec2.i == 0);
    REQUIRE(vec2.j == 1);
    REQUIRE(vec2.k == 0);

    vec = Vector(2, 1, 3);
    vec2 = vec.norm();

    REQUIRE(vec2.i == 2 / sqrtf(14));
    REQUIRE(vec2.j == 1 / sqrtf(14));
    REQUIRE(vec2.k == 3 / sqrtf(14));
}

TEST_CASE("Constant times a Vector", "[vector_times]") {
    Vector vec, vec2;

    vec = Vector(0, 1, 0);
    vec2 = vec.times(10);

    REQUIRE(vec2.i == 0);
    REQUIRE(vec2.j == 10);
    REQUIRE(vec2.k == 0);

    vec = Vector(2, 1, 3);
    vec2 = vec.times(-1);

    REQUIRE(vec2.i == -2);
    REQUIRE(vec2.j == -1);
    REQUIRE(vec2.k == -3);
}

TEST_CASE("Dot product between two vectors", "[vector_dot_product]") {
    REQUIRE(Vector(0, 1, 0).dot(Vector(1, 0, 0)) == 0);
    REQUIRE(Vector(0, 1, 0).dot(Vector(0, 0, 1)) == 0);
    REQUIRE(Vector(1, 0, 0).dot(Vector(0, 0, 1)) == 0);
    REQUIRE(Vector(1, 0, 0).dot(Vector(1, 0, 0)) == 1);
    REQUIRE(Vector(1, 2, 3).dot(Vector(1, 2, 3)) == 14);
    REQUIRE(Vector(-1, 2, 3).dot(Vector(1, 2, -3)) == -6);
}

TEST_CASE("Cross product between two vectors", "[vector_cross_product]") {
    Vector vec1, vec2, vec3;

    vec1 = Vector(0, 1, 0);
    vec2 = Vector(1, 0, 0);
    vec3 = vec1.cross(vec2);

    REQUIRE(vec3.i == 0);
    REQUIRE(vec3.j == 0);
    REQUIRE(vec3.k == -1);

    vec1 = Vector(0, 0, 1);
    vec2 = Vector(1, 0, 0);
    vec3 = vec1.cross(vec2);

    REQUIRE(vec3.i == 0);
    REQUIRE(vec3.j == 1);
    REQUIRE(vec3.k == 0);
}

TEST_CASE("Addition of two vectors", "[vector_plus]") {
    Vector vec1, vec2, vec3;

    vec1 = Vector(0, 1, 0);
    vec2 = Vector(1, 0, 0);
    vec3 = vec1.plus(vec2);

    REQUIRE(vec3.i == 1);
    REQUIRE(vec3.j == 1);
    REQUIRE(vec3.k == 0);

    vec1 = Vector(-4, -2, 1);
    vec2 = Vector(7, 2, -2);
    vec3 = vec1.plus(vec2);

    REQUIRE(vec3.i == 3);
    REQUIRE(vec3.j == 0);
    REQUIRE(vec3.k == -1);
}

TEST_CASE("Invert a Vector", "[vector_invert]") {
    Vector vec, vec2;

    vec = Vector(9, 1, -3.5f);
    vec2 = -vec;

    REQUIRE(vec2.i == -9);
    REQUIRE(vec2.j == -1);
    REQUIRE(vec2.k == 3.5);
}