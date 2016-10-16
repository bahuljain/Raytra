//
// Created by bahuljain on 10/4/16.
//

#ifndef RAYTRA_RAY_H
#define RAYTRA_RAY_H


#include <vector>
#include "Vector.h"
#include "Point.h"

class Ray {
public:
    Point origin;
    Vector direction;

    Ray(const Point &, const Vector &);

    Point getPointOnIt(float) const;
    float getOffsetFromOrigin(const Point &) const;
};


#endif //RAYTRA_RAY_H
