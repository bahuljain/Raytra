//
// Created by bahuljain on 10/3/16.
//

#ifndef RAYTRA_VECTOR_H
#define RAYTRA_VECTOR_H

class Vector {
public:
    float i, j, k;

    Vector();

    Vector(float, float, float);

    float mag() const;

    Vector norm() const;

    Vector times(float) const;

    Vector cross(const Vector &) const;

    float dot(const Vector &) const;

    Vector plus(const Vector &) const;

    Vector operator-() const;

    void printVector() const;
};

#endif //RAYTRA_VECTOR_H
