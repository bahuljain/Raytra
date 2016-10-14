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

Vector Vector::cross(const Vector &vec) const {
    float x, y, z;

    x = j * vec.k - k * vec.j;
    y = k * vec.i - i * vec.k;
    z = i * vec.j - j * vec.i;

    return Vector (x, y, z);
}

float Vector::dot(const Vector &vec) const {
    return i * vec.i + j * vec.j + k * vec.k;
}

float Vector::mag() const {
    return sqrtf(powf(i, 2.0) + powf(j, 2.0) + powf(k, 2.0));
}

Vector Vector::norm() const {
    float mag = this->mag();

    assert (mag != 0.0);

    return Vector(i/mag, j/mag, k/mag);
}

Vector Vector::operator-() const {
    return Vector(-i, -j, -k);
}

Vector Vector::times(float c) const {
    return Vector(i * c, j * c, k * c);
}

Vector Vector::plus(const Vector &vec) const {
    return Vector(i + vec.i, j + vec.j, k + vec.k);
}

void Vector::printVector() const {
    std::cout << i << "i + " << j << "j + " << k << "k" << std::endl;
}


