//
// Created by bahuljain on 10/10/16.
//

#include "include/AmbientLight.h"

AmbientLight::AmbientLight() {
    this->color = RGB(0, 0, 0);
}

void AmbientLight::setValues(float r, float g, float b) {
    this->color = RGB(r, g, b);
}
