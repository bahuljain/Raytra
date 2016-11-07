//
// Created by bahuljain on 10/4/16.
//

#ifndef RAYTRA_POINT_H
#define RAYTRA_POINT_H

#include "Vector.h"

class Point {
public:
    float x, y, z;

    Point() {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    };

    Point(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    };

    Vector sub(const Point &p) const {
        return Vector(x - p.x, y - p.y, z - p.z);
    };

    Point moveAlong(const Vector &vec) const {
        return Point(x + vec.i, y + vec.j, z + vec.k);
    };

    float distance2(const Point &p) const {
        return powf(x - p.x, 2.0) + powf(y - p.y, 2.0) + powf(z - p.z, 2.0);
    };

    void printPoint() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    };
};


#endif //RAYTRA_POINT_H
