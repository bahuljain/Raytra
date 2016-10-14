//
// Created by bahuljain on 10/3/16.
//

#include "include/Camera.h"

Camera::Camera() {
    this->eye = Point(0, 0, 0);

    /* Camera is pointing straight ahead*/
    Vector D = Vector(0.0, 0.0, -1.0f);

    this->w = - D;

    /* Note here D should not be [0 t 0] */
    this->u = Vector(1, 0, 0);
    this->v = Vector(0, 1, 0);

    this->d = 1;

    this->right = 0;
    this->left = 0;
    this->top = 0;
    this->bottom = 0;

    this->pw = 0;
    this->ph = 0;
}

void Camera::setValues(float x, float y, float z,
               float vx, float vy, float vz, float d,
               float iw, float ih, int pw, int ph) {

    this->eye = Point(x, y, z);

    /* Direction the camera is pointing to */
    Vector D = Vector(vx, vy, vz).norm();

    this->w = - D;

    /* Note here D should not be [0 t 0] */
    this->u = D.cross(Vector(0, 1, 0)).norm();
    this->v = D.cross(u).norm();

    this->d = d;

    this->right = iw / 2;
    this->left = - right;
    this->top = ih / 2;
    this->bottom = - top;

    this->pw = pw;
    this->ph = ph;
}

Point Camera::getPixelCenter(int i, int j,
                             float width, float height) const {
    Point center;

    float x = left + width * (i + 0.5f) / pw;
    float y = bottom + height * (j + 0.5f) / ph;

    center = eye.moveAlong(u.times(x))
                .moveAlong(v.times(y))
                .moveAlong(w.times(-d));

    return center;
}
