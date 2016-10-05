//
// Created by bahuljain on 10/5/16.
//

#ifndef RAYTRA_MATERIAL_H
#define RAYTRA_MATERIAL_H


#include "RGB.h"

class Material {
public:
    RGB diffuse;
    RGB specular;
    RGB ideal_specular;
    float roughness;

    Material();
    Material(float, float, float, float, float, float, float, float, float, float);
};


#endif //RAYTRA_MATERIAL_H
