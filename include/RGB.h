//
// Created by bahuljain on 10/5/16.
//

#ifndef RAYTRA_RGB_H
#define RAYTRA_RGB_H


class RGB {
public:
    float r, g, b;

    RGB() {
        this->r = 0.0;
        this->g = 0.0;
        this->b = 0.0;
    }

    RGB(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void setRGB(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};


#endif //RAYTRA_RGB_H
