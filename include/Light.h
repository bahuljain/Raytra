//
// Created by bahuljain on 10/10/16.
//

#ifndef RAYTRA_LIGHT_H
#define RAYTRA_LIGHT_H


#include "Point.h"
#include "RGB.h"

class Light {
private:
    char type;
public:
    Point position;
    RGB color;
    float intensity;

    virtual ~Light() {}

    void setType(char type) {
        this->type = type;
    }

    char getType() {
        return this->type;
    }
};


#endif //RAYTRA_LIGHT_H
