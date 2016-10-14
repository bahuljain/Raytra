//
// Created by bahuljain on 10/4/16.
//

#include "include/Ray.h"

Ray::Ray(const Point &origin, const Vector &direction) {
    this->origin = origin;
    this->direction = direction;
}

Point Ray::getPointOnIt(float t) const {
    return origin.moveAlong(direction.times(t));;
}
