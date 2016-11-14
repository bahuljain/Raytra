//
// Created by bahuljain on 10/10/16.
//

#ifndef RAYTRA_LIGHT_H
#define RAYTRA_LIGHT_H


#include "Point.h"
#include "RGB.h"

class Light {
public:
    RGB color;
    float intensity;

    virtual ~Light() {}
};

class PointLight : public Light {
public:
    Point position;

    PointLight(float x, float y, float z,
               float r, float g, float b, float I) {
        this->position = Point(x, y, z);
        this->color = RGB(r, g, b);
        this->intensity = I;
    };

    ~PointLight() {};
};

class SquareLight : public Light {
public:
    Point center;
    Vector u, v, w;
    float len;

    SquareLight(float x, float y, float z,
                float nx, float ny, float nz,
                float ux, float uy, float uz,
                float len, float r, float g, float b) {
        this->intensity = 1;
        this->center = Point(x, y, z);
        this->w = Vector(nx, ny, nz).norm();
        this->u = Vector(ux, uy, uz).norm();
        this->v = u.cross(w).norm();
        this->len = len;
        this->color = RGB(r, g, b);
    }

    ~SquareLight() {};

    /**
     * @name getLightSample
     * @brief returns a random point (within a given strata) on the area light
     *
     * @details The area light is broken down into blocks (or stratas) from
     * which a random point is chosen for sampling. For example given a value
     * of 2 for @param strata the total number of blocks in the area light would
     * be equal to the sqaure of @param strata.
     *
     * @param p - column index for the block of the stratified area light.
     * @param q - row index of the block of the stratified area light.
     * @param strata - total number of divisions along one axis. Total number
     * of blocks equal to the square of this value
     */
    Point getLightSample(int p, int q, int strata) {
        Point sample;

        float u_d = (p + ((float) rand() / RAND_MAX)) / strata;
        float v_d = (q + ((float) rand() / RAND_MAX)) / strata;

        return center
                .moveAlong(u.times((u_d - 0.5f) * len))
                .moveAlong(v.times((v_d - 0.5f) * len));
    }
};


class AmbientLight : public Light {
public:
    AmbientLight() {
        this->color = RGB(0, 0, 0);
        this->intensity = 1;
    };

    AmbientLight(float r, float g, float b) {
        this->color = RGB(r, g, b);
        this->intensity = 1;
    };

    ~AmbientLight() {};
};


#endif //RAYTRA_LIGHT_H
