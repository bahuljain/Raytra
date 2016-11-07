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
    BVHNode *root;

    BVHNode *_makeBVHTree(std::vector<BoundingBox *> &, int, int, int) const;

    bool _isIntercepted(const BVHNode *node,
                        const std::vector<Surface *> &surfaces,
                        const Ray &ray,
                        float t_max,
                        int origin_surface_idx,
                        int mode) const;

    std::tuple<int, float>
    _getClosestSurface(const BVHNode *node,
                       const std::vector<Surface *> &surfaces,
                       const Ray &ray,
                       int origin_surface_idx,
                       int mode) const;

    void printTree(BVHNode *node) const;

    int _getMaxHeight(BVHNode *node) const;

public:

    BVHTree();

    ~BVHTree();

    int makeBVHTree(const std::vector<Surface *> &surfaces);

    bool isIntercepted(const Ray &ray,
                       const std::vector<Surface *> &surfaces,
                       float t_max,
                       int origin_surface_idx,
                       int mode) const;

    std::tuple<int, float>
    getClosestSurface(const std::vector<Surface *> &surfaces,
                      const Ray &ray,
                      int origin_surface_idx,
                      int mode) const;

    bool isEmpty() const;

    int getMaxHeight();

    void printTree() const;

};


#endif //RAYTRA_BVHTREE_H
