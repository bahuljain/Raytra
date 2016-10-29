//
// Created by bahuljain on 10/4/16.
//

#ifndef RAYTRA_SPHERE_H
#define RAYTRA_SPHERE_H


#include "Surface.h"
#include "Point.h"

class Sphere : public Surface {
public:
    Point center;
    float radius;

    Sphere(float, float, float, float);

    ~Sphere() {};

    virtual float getIntersection(const Ray &) const;

    virtual Vector getSurfaceNormal(const Point &) const;

    virtual bool isFrontFacedTo(const Ray &) const;

    virtual BoundingBox getBoundingBox() const;

};


#endif //RAYTRA_SPHERE_H
