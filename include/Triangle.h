//
// Created by bahuljain on 10/6/16.
//

#ifndef RAYTRA_TRIANGLE_H
#define RAYTRA_TRIANGLE_H


#include "Surface.h"

class Triangle : public Surface {
private:
    Vector normal;
public:
    Point a;
    Point b;
    Point c;

    Triangle(float, float, float, float, float, float, float, float, float);

    ~Triangle() {};

    float getIntersection(const Ray &) const;

    Vector getSurfaceNormal(const Point &) const;

    bool isFrontFacedTo(const Ray &) const;
};


#endif //RAYTRA_TRIANGLE_H
