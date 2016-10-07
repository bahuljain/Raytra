//
// Created by bahuljain on 10/6/16.
//

#include "include/Triangle.h"

Triangle::Triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3) {
    this->p1 = Point(x1, y1, z1);
    this->p2 = Point(x2, y2, z2);
    this->p3 = Point(x3, y3, z3);
}

float Triangle::getIntersection(const Ray &) const {


    return 0;
}
