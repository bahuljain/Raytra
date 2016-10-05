//
// Created by bahuljain on 10/4/16.
//

#ifndef RAYTRA_SPHERE_H
#define RAYTRA_SPHERE_H


#include "Surface.h"
#include "Point.h"

class Sphere : public Surface {
public:
    Point center = Point();
    float radius;

    Sphere(float, float, float, float);
    virtual int getIntersection(const Ray &) const;
};


#endif //RAYTRA_SPHERE_H
