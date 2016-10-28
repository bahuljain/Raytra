//
// Created by bahuljain on 10/28/16.
//

#include "include/BoundingBox.h"

BoundingBox::BoundingBox(float x_min, float x_max,
                         float y_min, float y_max,
                         float z_min, float z_max) {
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
    this->z_min = z_min;
    this->z_max = z_max;

    center = Point((x_min + x_max) / 2,
                   (y_min + y_max) / 2,
                   (z_min + z_max) / 2);
}

void BoundingBox::setBoundedSurface(int index) {
    this->bounded_surface_idx = index;
}

int BoundingBox::getBoundedSurface() {
    return this->bounded_surface_idx;
}
