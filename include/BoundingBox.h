//
// Created by bahuljain on 10/28/16.
//

#ifndef RAYTRA_BOUNDINGBOX_H
#define RAYTRA_BOUNDINGBOX_H


#include "Point.h"
#include "Ray.h"

class BoundingBox {
private:
    int bounded_surface_idx;

public:
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z_min;
    float z_max;

    Point center;

    BoundingBox(float, float, float, float, float, float);

    void setBoundedSurface(int index);

    int getBoundedSurface();

    virtual bool intersects(const Ray &) const;
};


#endif //RAYTRA_BOUNDINGBOX_H
