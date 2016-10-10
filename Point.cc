//
// Created by bahuljain on 10/4/16.
//

#include "include/Point.h"
#include <math.h>


Point::Point() {
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}

Point::Point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Point Point::moveAlong(const Vector &vec) {
    return Point(x + vec.i, y + vec.j, z + vec.k);
}

Vector Point::sub(const Point &p) const {
    return Vector(x - p.x, y - p.y, z - p.z);
}

float Point::dot(const Vector &vec) const {
    return x * vec.i + y * vec.j + z * vec.k;
}

float Point::distance2(const Point &p) const {
    return powf(x - p.x, 2.0) + powf(y - p.y, 2.0) + powf(z - p.z, 2.0);
}

