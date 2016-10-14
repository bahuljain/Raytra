//
// Created by bahuljain on 10/10/16.
//

#ifndef RAYTRA_LIGHT_H
#define RAYTRA_LIGHT_H


#include "Point.h"
#include "RGB.h"

class Light {
public:
    Point position;
    RGB color;
    float intensity;

    Light();

    void setValues(float, float, float, float, float, float, float);

};


#endif //RAYTRA_LIGHT_H
