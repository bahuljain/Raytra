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
    Vector sub(const Point &) const ;
    Point moveAlong(const Vector &) const;
    float dot(const Vector &) const;
    float distance2(const Point &) const;
    void printPoint() const;
};


#endif //RAYTRA_POINT_H
