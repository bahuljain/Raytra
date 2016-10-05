//
// Created by bahuljain on 10/4/16.
//

#include "Point.h"



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

Vector Point::sub(Point p) {
    return Vector(x - p.x, y - p.y, z - p.z);
}
