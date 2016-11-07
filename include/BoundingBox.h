//
// Created by bahuljain on 10/28/16.
//

#ifndef RAYTRA_BOUNDINGBOX_H
#define RAYTRA_BOUNDINGBOX_H


#include <functional>
#include "Point.h"
#include "Ray.h"

class BoundingBox {
private:
    int bounded_surface_idx;

public:
    int id;
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z_min;
    float z_max;

    Point center;

    BoundingBox();

    BoundingBox(float x_min, float x_max,
                float y_min, float y_max,
                float z_min, float z_max);

    void setBoundedSurface(int index);

    int getBoundedSurface() const;

    float getIntersection(const Ray &ray) const;

    Vector getSurfaceNormal(const Point &p) const;

    static BoundingBox *groupBoundingBoxes(std::vector<BoundingBox *> &,
                                           int, int);

    static std::function<bool(BoundingBox *,
                              BoundingBox *)> compare(int);

    void printBox() const;

};


#endif //RAYTRA_BOUNDINGBOX_H
