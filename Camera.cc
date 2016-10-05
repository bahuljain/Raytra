//
// Created by bahuljain on 10/3/16.
//

#include "include/Camera.h"

Camera::Camera(float x, float y, float z,
               float vx, float vy, float vz, float d,
               float iw, float ih, int pw, int ph) {

    this->eye = Point(x, y, z);

    /* Direction the camera is pointing to */
    Vector D = Vector(vx, vy, vz);

    this->w = - D;

    /* Note here D should not be [0 t 0] */
    this->u = D.cross(Vector(0, 1, 0));
    this->v = u.cross(D);

    this->d = d;

    this->right = iw / 2;
    this->left = - right;
    this->top = ih / 2;
    this->bottom = - top;

    this->pw = pw;
    this->ph = ph;
}

Point Camera::getPixelCenter(int i, int j, float width, float height) {
    float x = left + width * (i + 0.5) / pw;
    float y = bottom + height * (j + 0.5) / ph;

    Point center = eye.moveAlong(w.times(-d))
                    .moveAlong(u.times(x))
                    .moveAlong(v.times(y));

    return center;
}
