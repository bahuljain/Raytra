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

class PointLight : public Light {
public:
    PointLight(float x, float y, float z,
               float r, float g, float b, float I) {
        this->position = Point(x, y, z);
        this->color = RGB(r, g, b);
        this->intensity = I;

        this->setType('p');
    };

    ~PointLight() {};
};

class SquareLight: public Light {
public:
    Point center;
    Vector u, v, w;
    float len;

    SquareLight(float x, float y, float z,
                float nx, float ny, float nz,
                float ux, float uy, float uz,
                float len, float r, float g, float b) {
        this->center = Point(x, y, z);

        this->w = Vector(nx, ny, nz).norm();
        this->u = Vector(ux, uy, uz).norm();
        this->v = u.cross(w).norm();

        this->len = len;

        this->color = RGB(r, g, b);

        this->setType('s');

    }

    ~SquareLight() {};
};


class AmbientLight : public Light {
public:
    AmbientLight() {
        this->color = RGB(0, 0, 0);
        this->intensity = 1;
        this->setType('a');
    };

    AmbientLight(float r, float g, float b) {
        this->color = RGB(r, g, b);
        this->intensity = 1;
        this->setType('a');
    };

    ~AmbientLight() {};
};


#endif //RAYTRA_LIGHT_H
