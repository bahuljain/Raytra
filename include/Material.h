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
    float phong;

    Material() {
        this->diffuse = RGB(0, 0, 0);
        this->specular = RGB(0, 0, 0);
        this->ideal_specular = RGB(0, 0, 0);
        this->phong = 1;
    };

    Material(float dr, float dg, float db,
             float sr, float sg, float sb,
             float r, float ir, float ig, float ib) {
        this->diffuse = RGB(dr, dg, db);
        this->specular = RGB(sr, sg, sb);
        this->ideal_specular = RGB(ir, ig, ib);
        this->phong = r;
    };
};


#endif //RAYTRA_MATERIAL_H
