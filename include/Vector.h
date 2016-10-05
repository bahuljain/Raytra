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
    float mag();
    Vector norm();
    Vector times(float);
    Vector cross(const Vector &);
    float dot(const Vector &) const ;
    Vector operator-();
};

#endif //RAYTRA_VECTOR_H
