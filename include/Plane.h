//
// Created by bahuljain on 10/6/16.
//

#ifndef RAYTRA_PLANE_H
#define RAYTRA_PLANE_H


#include "Surface.h"

class Plane : public Surface {
public:
    Vector normal;
    float d;

    Plane(float, float, float, float);

    ~Plane() {};

    virtual float getIntersection(const Ray &) const;

    virtual Vector getSurfaceNormal(const Point &) const;

    virtual bool isFrontFacedTo(const Vector &) const;
};


#endif //RAYTRA_PLANE_H
