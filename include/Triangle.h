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

    virtual float getIntersection(const Ray &) const;

    virtual Vector getSurfaceNormal(const Point &) const;

    virtual bool isFrontFacedTo(const Ray &) const;
};


#endif //RAYTRA_TRIANGLE_H
