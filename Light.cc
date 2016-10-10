//
// Created by bahuljain on 10/10/16.
//

#include "include/Light.h"

void Light::setValues(float x, float y, float z,
                      float r, float g, float b, float I) {
    this->position = Point(x, y, z);
    this->color = RGB(r, g, b);
    this->intensity = I;
}

Light::Light() {
    this->position = Point(0, 0, 0);
    this->color = RGB(0, 0, 0);
    this->intensity = 1.0;
}
