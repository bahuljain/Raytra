//
// Created by bahuljain on 10/29/16.
//

#ifndef RAYTRA_BVHNODE_H
#define RAYTRA_BVHNODE_H


#include "BoundingBox.h"

class BVHNode {
public:
    BoundingBox thisBound;

    BVHNode *left;
    BVHNode *right;

    BVHNode() {
        this->left = NULL;
        this->right = NULL;
    };

    ~BVHNode() {
        delete this->left;
        delete this->right;
    };
};


#endif //RAYTRA_BVHNODE_H
