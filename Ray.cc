//
// Created by bahuljain on 10/4/16.
//

#include <iostream>
#include "include/Ray.h"

Ray::Ray(const Point &origin, const Vector &direction) {
    this->origin = origin;
    this->direction = direction;
}

Point Ray::getPointOnIt(float t) const {
    return origin.moveAlong(direction.times(t));;
}

/*
 * This function assumes that the given point lies on this ray
 */
float Ray::getOffsetFromOrigin(const Point &p) const {
    Vector dir = p.sub(origin);

    if (direction.i != 0)
        return dir.i / direction.i;

    if (direction.j != 0)
        return dir.j / direction.j;

    if (direction.k != 0)
        return dir.k / direction.k;

    std::cout << "error: Given point does not lie on this ray" << std::endl;
    return -1;
}