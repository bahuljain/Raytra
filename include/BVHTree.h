//
// Created by bahuljain on 10/29/16.
//

#ifndef RAYTRA_BVHTREE_H
#define RAYTRA_BVHTREE_H


#include <functional>
#include "Surface.h"
#include "BoundingBox.h"

class BVHNode {
public:
    BoundingBox *thisBound;

    BVHNode *left;
    BVHNode *right;

    BVHNode() {
        this->thisBound = nullptr;
        this->left = nullptr;
        this->right = nullptr;
    };

    ~BVHNode() {
        delete this->thisBound;
        delete this->left;
        delete this->right;
    };
};

class BVHTree {
private:
    BVHNode *root;
    const std::vector<Surface *> *surfaces;

    BVHNode *_makeBVHTree(std::vector<BoundingBox *> &bboxes, int start,
                          int end, int axis) const;

    bool _isIntercepted(const BVHNode *node, const Ray &ray,
                        float t_max, int mode) const;

    std::tuple<int, float>
    _getClosestSurface(const BVHNode *node, const Ray &ray,
                       int mode, const std::tuple<int, float> &closest) const;

    void printTree(BVHNode *node) const;

    int _getMaxHeight(BVHNode *node) const;

public:

    BVHTree(const std::vector<Surface *> *surfaces);

    ~BVHTree();

    int size() const;

    Surface *at(int index) const;

    int makeBVHTree();

    bool isIntercepted(const Ray &ray, float t_max, int mode) const;

    std::tuple<int, float> getClosestSurface(const Ray &ray, int mode) const;

    bool isEmpty() const;

    int getMaxHeight();

    void printTree() const;
};

#endif //RAYTRA_BVHTREE_H
