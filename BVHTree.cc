//
// Created by bahuljain on 10/29/16.
//

#include "include/BVHTree.h"
#include <algorithm>

BVHTree::BVHTree() {
    this->root = NULL;
}

BVHTree::~BVHTree() {
    delete this->root;
}

/**
 * @name makeBVHTree
 * @brief
 *
 * @param surfaces -
 * @returns
 */
int BVHTree::makeBVHTree(const std::vector<Surface *> &surfaces) {
    std::vector<BoundingBox> bboxes;

    for (unsigned int i = 0; i < surfaces.size(); i++) {
        BoundingBox bbox = surfaces[i]->getBoundingBox();

        bbox.setBoundedSurface(i);
        bboxes.push_back(bbox);
    }

    this->root = this->makeBVHTree(bboxes, 0, (int) (bboxes.size() - 1), 0);

    return (this->root != NULL);

}

BVHNode *BVHTree::makeBVHTree(std::vector<BoundingBox> &bboxes,
                              int start, int end, int axis) const {
    BVHNode *node;

    if (start > end)
        return NULL;

    node = new BVHNode();

    /**
     * If only one bounding box remains just make that bounding box as
     * thisBound for @var node. Left and right pointers will remain null.
     */
    if (start == end) {
        node->thisBound = bboxes[0];
        return node;
    }

    /**
     * Should be a BoundingBox bounding all the bounding boxes in @param bboxes
     * from @param start to @param end.
     */
    node->thisBound = BoundingBox::groupBoundingBoxes(bboxes, start, end);

    std::nth_element(bboxes.begin() + start,
                     bboxes.begin() + (end - start) / 2,
                     bboxes.begin() + end,
                     BoundingBox::compare(axis));

    int mid = start + (end - start) / 2;

    node->left = this->makeBVHTree(bboxes, start, mid, (axis + 1) % 3);
    node->right = this->makeBVHTree(bboxes, mid + 1, end, (axis + 1) % 3);

    return node;
}


bool BVHTree::isIntercepted(const Ray &ray,
                            const std::vector<Surface *> &surfaces,
                            float t_max,
                            int origin_surface_idx) const {
        return this->isIntercepted(this->root,
                               surfaces, ray, t_max,
                               origin_surface_idx);
}

bool BVHTree::isIntercepted(const BVHNode *node,
                            const std::vector<Surface *> &surfaces,
                            const Ray &ray,
                            float t_max,
                            int origin_surface_idx) const {
    if (node == NULL)
        return false;

    /*
     * If the bounding box doesn't intercepts the ray don't bother going any
     * further.
     */
    if (!node->thisBound.intersects(ray))
        return false;

    /*
     * If both left and right nodes are NULL, the node is a leaf node and we
     * need to compute intersections with the surface
     */
    if (node->left == NULL && node->right == NULL) {
        int surface_idx = node->thisBound.getBoundedSurface();

        if (surface_idx == origin_surface_idx)
            return false;

        float t = surfaces[surface_idx]->getIntersection(ray);

        return (t >= 0 && t < t_max);
    }

    /*
     * At this point we know that ray intersects thisBound, so we need to
     * traverse the left and right nodes to find intersections. If either one
     * of the node returns an interception return true.
     */
    return isIntercepted(node->left, surfaces, ray,
                         t_max, origin_surface_idx) ||
            isIntercepted(node->right, surfaces, ray,
                          t_max, origin_surface_idx);
}
