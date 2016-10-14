//
// Created by bahuljain on 10/5/16.
//

#include "include/Material.h"

Material::Material() {
    this->diffuse = RGB(0, 0, 0);
    this->specular = RGB(0, 0, 0);
    this->ideal_specular = RGB(0, 0, 0);
    this->phong = 1;
}

Material::Material(float dr, float dg, float db,
                   float sr, float sg, float sb,
                   float r, float ir, float ig, float ib) {
    this->diffuse = RGB(dr, dg, db);
    this->specular = RGB(sr, sg, sb);
    this->ideal_specular = RGB(ir, ig, ib);
    this->phong = r;
}