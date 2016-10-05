//
// Created by bahuljain on 10/3/16.
//

#ifndef RAYTRA_CAMERA_H
#define RAYTRA_CAMERA_H


#include "Point.h"

class Camera {
public:
    Point e = Point();
    Vector w = Vector();
    Vector u = Vector();
    Vector v = Vector();

    float d, pw, ph;

    float l, r, t, b;

    Camera(float, float, float, float, float, float, float, float, float, float, float);

};


#endif //RAYTRA_CAMERA_H
