//
// Created by bahuljain on 10/3/16.
//

#include "Camera.h"

Camera::Camera(float x, float y, float z,
               float vx, float vy, float vz, float d,
               float iw, float ih, float pw, float ph) {

    this->e = Point(x, y, z);

    /* Direction the camera is pointing to */
    Vector D = Vector(vx, vy, vz);

    this->w = - D;

    /* Note here D should not be [0 t 0] */
    this->u = D.cross(Vector(0, 1, 0));
    this->v = u.cross(D);

    this->d = d;

    this->r = iw / 2;
    this->l = - r;
    this->t = ih / 2;
    this->b = - t;

    this->pw = pw;
    this->ph = ph;
}
