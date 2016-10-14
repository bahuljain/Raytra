//
// Created by bahuljain on 10/3/16.
//

#ifndef RAYTRA_CAMERA_H
#define RAYTRA_CAMERA_H


#include "Point.h"

class Camera {
public:
    Point eye;
    Vector w;
    Vector u;
    Vector v;

    float d;
    int pw, ph;

    float left, right, top, bottom;

    Camera();
    void setValues(float, float, float, float, float, float, float, float, float, int, int);

    Point getPixelCenter(int, int, float, float) const;
};


#endif //RAYTRA_CAMERA_H
