//
// Created by bahuljain on 10/6/16.
//

#ifndef RAYTRA_TRIANGLE_H
#define RAYTRA_TRIANGLE_H


#include "Surface.h"

class Triangle : public Surface {
public:
    Point p1;
    Point p2;
    Point p3;

    Triangle(float, float, float, float, float, float, float, float, float);

    ~Triangle() {};

    virtual float getIntersection(const Ray &) const;

    virtual Vector getSurfaceNormal(const Point &) const;
};


#endif //RAYTRA_TRIANGLE_H
