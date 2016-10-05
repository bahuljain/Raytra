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
    Vector norm(Vector);
    Vector cross(Vector);
    float dot(Vector);
    Vector operator-();
};


#endif //RAYTRA_VECTOR_H
