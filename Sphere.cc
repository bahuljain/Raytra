//
// Created by bahuljain on 10/4/16.
//

#include <math.h>
#include "include/Sphere.h"

Sphere::Sphere(float x, float y, float z, float r) {
    this->center = Point (x, y, z);
    this->radius = r;
}

int Sphere::getIntersection(const Ray &ray) const {
    Vector x = ray.origin.sub(center);
    float dd = ray.direction.dot(ray.direction);

    float discriminant = powf(ray.direction.dot(x), 2.0)
                         - dd * (x.dot(x) - powf(radius, 2.0));

    if (discriminant < 0)
        return -1;

    float t1 = (- ray.direction.dot(x) + sqrtf(discriminant)) / dd;
    float t2 = (- ray.direction.dot(x) - sqrtf(discriminant)) / dd;

    return fminf(t1, t2);
}
