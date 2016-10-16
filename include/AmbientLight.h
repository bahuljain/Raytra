//
// Created by bahuljain on 10/10/16.
//

#ifndef RAYTRA_AMBIENTLIGHT_H
#define RAYTRA_AMBIENTLIGHT_H


#include "RGB.h"
#include "Light.h"

class AmbientLight : public Light {
public:
    AmbientLight(float, float, float);

    ~AmbientLight() {};
};


#endif //RAYTRA_AMBIENTLIGHT_H
