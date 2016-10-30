//
// Created by bahuljain on 10/29/16.
//

#ifndef RAYTRA_BVHTREE_H
#define RAYTRA_BVHTREE_H


#include <functional>
#include "BVHNode.h"
#include "Surface.h"

class BVHTree {
private:
    BVHNode* makeBVHTree(std::vector<BoundingBox> &, int, int, int) const;

    bool isIntercepted(const BVHNode *node,
                       const std::vector<Surface *> &,
                       const Ray &, float, int) const;

    std::tuple<int, float> getClosestSurface(const BVHNode *node,
                                             const std::vector<Surface *> &,
                                             const Ray &, int) const;

    void printTree(BVHNode *node) const;

public:
    BVHNode *root;

    BVHTree();

    ~BVHTree();

    int makeBVHTree(const std::vector<Surface *> &);

    bool isIntercepted(const Ray &,
                       const std::vector<Surface *> &,
                       float, int) const;

    std::tuple<int, float> getClosestSurface(const std::vector<Surface *> &,
                                             const Ray &, int) const;

    void printTree() const;

};


#endif //RAYTRA_BVHTREE_H
