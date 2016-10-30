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
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z_min;
    float z_max;

    Point center;

    BoundingBox();

    BoundingBox(float, float, float, float, float, float);

    void setBoundedSurface(int index);

    int getBoundedSurface() const ;

    bool intersects(const Ray &) const;

    static BoundingBox groupBoundingBoxes(std::vector<BoundingBox> &,
                                          int, int);

    static std::function<bool(const BoundingBox &,
                              const BoundingBox &)> compare(int);

};


#endif //RAYTRA_BOUNDINGBOX_H
