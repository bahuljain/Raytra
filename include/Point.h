//
// Created by bahuljain on 10/4/16.
//

#ifndef RAYTRA_POINT_H
#define RAYTRA_POINT_H


#include "Vector.h"

class Point {
public:
    float x, y, z;
    Point();
    Point(float, float, float);
    Vector sub(const Point &);
    Point moveAlong(const Vector &);
};


#endif //RAYTRA_POINT_H
