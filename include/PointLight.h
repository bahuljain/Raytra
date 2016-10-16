//
// Created by bahuljain on 10/16/16.
//

#ifndef RAYTRA_POINTLIGHT_H
#define RAYTRA_POINTLIGHT_H


#include "Light.h"

class PointLight : public Light {
public:
    PointLight(float, float, float, float, float, float, float);

    ~PointLight() {};
};


#endif //RAYTRA_POINTLIGHT_H
