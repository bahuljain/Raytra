//
// Created by bahuljain on 10/6/16.
//

#ifndef RAYTRA_TRIANGLE_H
#define RAYTRA_TRIANGLE_H


#include "Surface.h"

class Triangle : public Surface {
public:
    bool isInMesh;
    Point p1;
    Point p2;
    Point p3;
    Vector normal;
    Vector n1, n2, n3;
    float _a, _b, _c, _d, _e, _f;

    Triangle(float, float, float, float, float, float, float, float, float);

    ~Triangle() {};

    float getIntersection(const Ray &) const;

    Vector getSurfaceNormal(const Point &) const;

    bool isFrontFacedTo(const Ray &) const;
};


#endif //RAYTRA_TRIANGLE_H
