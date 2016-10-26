//
// Created by bahuljain on 10/16/16.
//

#include "include/PointLight.h"

PointLight::PointLight(float x, float y, float z,
                       float r, float g, float b, float I) {
    this->position = Point(x, y, z);
    this->color = RGB(r, g, b);
    this->intensity = I;

    this->setType('p');
}