//
// Created by bahuljain on 10/3/16.
//

#include <math.h>
#include "Vector.h"

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

Vector Vector::cross(Vector vec) {
    float x, y, z;

    x = j * vec.k - k * vec.j;
    y = i * vec.k - k * vec.i;
    z = i * vec.j - j * vec.i;

    return Vector (x, y, z);
}

float Vector::dot(Vector vec) {
    return i * vec.i + j * vec.j + k * vec.k;
}

/*Vector Vector::operator-(Vector vec) {
    return Vector(-vec.i, -vec.j, -vec.k);
}*/

float Vector::mag() {
    return sqrtf(powf(i, 2.0) + powf(j, 2.0) + powf(k, 2.0));
}

Vector Vector::norm(Vector vec) {
    float mag = vec.mag();
    return Vector(i/mag, j/mag, k/mag);
}

Vector Vector::operator-() {
    return Vector(-i, -j, -k);
}

