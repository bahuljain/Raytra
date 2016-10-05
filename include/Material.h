//
// Created by bahuljain on 10/5/16.
//

#ifndef RAYTRA_MATERIAL_H
#define RAYTRA_MATERIAL_H


#include "RGB.h"

class Material {
public:
    RGB diffuse = RGB ();
    RGB specular = RGB ();
    RGB ideal_specular = RGB ();
    float roughness;

    Material(float, float, float, float, float, float, float, float, float, float);
};


#endif //RAYTRA_MATERIAL_H
