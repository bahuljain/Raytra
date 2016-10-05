//
// Created by bahuljain on 10/3/16.
//

#ifndef RAYTRA_CAMERA_H
#define RAYTRA_CAMERA_H


#include "Point.h"

class Camera {
public:
    Point eye = Point();
    Vector w = Vector();
    Vector u = Vector();
    Vector v = Vector();

    float d;
    int pw, ph;

    float left, right, top, bottom;

    Camera(float, float, float, float, float, float, float, float, float, int, int);
    Point getPixelCenter(int, int, float, float);
};


#endif //RAYTRA_CAMERA_H
