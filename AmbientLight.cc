//
// Created by bahuljain on 10/10/16.
//

#include "include/AmbientLight.h"

AmbientLight::AmbientLight(float r, float g, float b) {
    this->color = RGB(r, g, b);
    this->position = Point(0, 0, 0);
    this->intensity = 1;

    this->setType('a');
}
