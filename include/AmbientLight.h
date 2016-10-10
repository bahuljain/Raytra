//
// Created by bahuljain on 10/10/16.
//

#ifndef RAYTRA_AMBIENTLIGHT_H
#define RAYTRA_AMBIENTLIGHT_H


#include "RGB.h"

class AmbientLight {
public:
    RGB color;

    AmbientLight();
    void setValues(float, float, float);
};


#endif //RAYTRA_AMBIENTLIGHT_H
