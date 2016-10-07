//
// Created by bahuljain on 10/3/16.
//

#include <math.h>
#include "include/Vector.h"
#include <iostream>
#include <assert.h>

Vector::Vector() {
    this->i = 0;
    this->j = 0;
    this->k = 0;
}

Vector::Vector(float i, float j, float k) {
    this->i = i;
    this->j = j;
    this->k = k;
}

Vector Vector::cross(const Vector &vec) {
    float x, y, z;

    x = j * vec.k - k * vec.j;
    y = i * vec.k - k * vec.i;
    z = i * vec.j - j * vec.i;

    return Vector (x, y, z);
}

float Vector::dot(const Vector &vec) const {
    return i * vec.i + j * vec.j + k * vec.k;
}

float Vector::mag() {
    return sqrtf(powf(i, 2.0) + powf(j, 2.0) + powf(k, 2.0));
}

Vector Vector::norm() {
    float mag = this->mag();

    assert (mag != 0.0);

    return Vector(i/mag, j/mag, k/mag);
}

Vector Vector::operator-() {
    return Vector(-i, -j, -k);
}

Vector Vector::times(float c) {
    return Vector(i * c, j * c, k * c);
}

