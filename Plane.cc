//
// Created by bahuljain on 10/6/16.
//

#include "include/Plane.h"

Plane::Plane(float nx, float ny, float nz, float d) {
    this->normal = Vector(nx, ny, nz).norm();
    this->d = d;
}

float Plane::getIntersection(const Ray &ray) const {
    float a = ray.direction.dot(normal);

    if (a == 0)
        return -1;

    float t = -(ray.origin.dot(normal) - d) / a;

    return t;
}

Vector Plane::getSurfaceNormal(const Point &p) const {
    return normal;
}
