//
// Created by bahuljain on 10/29/16.
//

#include "include/BVHTree.h"
#include <algorithm>

using namespace std;

BVHTree::BVHTree() {
    this->root = NULL;
}

BVHTree::~BVHTree() {
    delete this->root;
}

/**
 * @name    makeBVHTree
 * @brief   Given a list of surfaces it creates a Bounding Volume
 *          Hierarchical Tree structure.
 *
 * @param surfaces - a list of surfaces that need to be converted to a BVHTree.
 * @returns          a BVHTree for the corresponding list of surfaces.
 */
int BVHTree::makeBVHTree(const vector<Surface *> &surfaces) {
    vector<BoundingBox> bboxes;

    cout << "Constructing a bounding box for each surface. ";

    /*
     * A list of BoundingBox objects are created each corresponding to a
     * surface.
     */
    for (unsigned int i = 0; i < surfaces.size(); i++) {
        BoundingBox bbox = surfaces[i]->getBoundingBox();

        bbox.setBoundedSurface(i);
        bboxes.push_back(bbox);
    }

    cout << "[Done]" << endl << "Constructing BVH Tree. ";

    this->root = this->makeBVHTree(bboxes, 0, (int) (bboxes.size() - 1), 0);

    cout << "[Done]" << endl;

    return (this->root != NULL);

}

/**
 * @name    makeBVHTree
 * @private used in BVHTree class only
 * @brief   Given a list of bounding boxes and a section given by the start
 *          and end indices it forms a BVHTree for the same.
 *
 * @param bboxes - list of bounding boxes a part of which needs to be
 *                 converted to a BVHTree node.
 * @param start  - start index of the section to be considered in bboxes list.
 * @param end    - end index of the section to be considered in bboxes list.
 * @param axis   - the axis along which the bounding boxes need to
 *                 partially sorted.
 * @returns        a pointer to the root of the BVHTree constructed.
 *
 * @details BoundingBoxes are partially sorted at each step (based on a given
 * axis which is chosen in a round-robin fashion) and then split into two groups
 * on which the same process is applied recursively. At each step a new
 * BoundingBox is created for the entire group until the group contains only
 * a single BoundingBox.
 */
BVHNode* BVHTree::makeBVHTree(vector<BoundingBox> &bboxes,
                              int start, int end, int axis) const {
    BVHNode *node;

    if (start > end) {
        cout << "Should never reach here" << endl;
        return NULL;
    }

    node = new BVHNode();

    /**
     * If only one bounding box remains just make that bounding box as
     * thisBound for @var node. Left and right pointers will remain null.
     */
    if (start == end) {
        node->thisBound = bboxes[start];
        return node;
    }

    /**
     * Should be a BoundingBox bounding all the bounding boxes in @param bboxes
     * from @param start to @param end.
     */
    node->thisBound = BoundingBox::groupBoundingBoxes(bboxes, start, end);

    /**
     * Partially sort for the middle element.
     * If odd elements the middle element is the [(n + 1) / 2]th element
     * If even elements the middle element in the [n / 2 + 1]th element
     */
    nth_element(bboxes.begin() + start,
                bboxes.begin() + (end - start + 1) / 2,
                bboxes.begin() + end + 1,
                BoundingBox::compare(axis));

    int mid = start + (end - start + 1) / 2;

    /*
     * Recursively create tree for the left and right partitions from the
     * middle element.
     */
    node->left = this->makeBVHTree(bboxes, start, mid - 1, (axis + 1) % 3);
    node->right = this->makeBVHTree(bboxes, mid, end, (axis + 1) % 3);

    return node;
}


bool BVHTree::isIntercepted(const Ray &ray,
                            const vector<Surface *> &surfaces,
                            float t_max,
                            int origin_surface_idx) const {
    return this->isIntercepted(this->root,
                               surfaces, ray, t_max,
                               origin_surface_idx);
}

bool BVHTree::isIntercepted(const BVHNode *node,
                            const vector<Surface *> &surfaces,
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

tuple<int, float>
BVHTree::getClosestSurface(const vector<Surface *> &surfaces,
                           const Ray &ray,
                           int origin_surface_idx) const {
    return getClosestSurface(this->root, surfaces, ray, origin_surface_idx);
}

tuple<int, float>
BVHTree::getClosestSurface(const BVHNode *node,
                           const vector<Surface *> &surfaces,
                           const Ray &ray,
                           int origin_surface_idx) const {
    tuple<int, float> invalid =
            make_tuple(-1, numeric_limits<float>::infinity());

    if (node == NULL)
        return invalid;

    /*
     * If bounding box doesn't intersect with ray then dont bother going
     * further and just return (-1, infinity)
     */
    if (!node->thisBound.intersects(ray))
        return invalid;

    /*
     * If both left and right nodes are NULL, the node is a leaf node and we
     * need to compute intersections with the actual surface. If there is an
     * actual intersection with the surface then return those values else
     * return (-1, infinity).
     */
    if (node->left == NULL && node->right == NULL) {
        int surface_idx = node->thisBound.getBoundedSurface();

        if (surface_idx == origin_surface_idx || surface_idx == -1)
            return invalid;

        float t = surfaces[surface_idx]->getIntersection(ray);

        return (t >= 0)
               ? make_tuple(surface_idx, t)
               : invalid;
    }

    /*
     * At this point we know the bounding box intersects the ray, so now we
     * check for intersections with the left and right nodes and get the
     * closest surface from each side. Between the two of them return the one
     * that is closer.
     */
    tuple<int, float> closest_surface_left, closest_surface_right;

    closest_surface_left = this->getClosestSurface(node->left, surfaces,
                                                   ray, origin_surface_idx);

    closest_surface_right = this->getClosestSurface(node->right, surfaces,
                                                    ray, origin_surface_idx);

    return (get<1>(closest_surface_left) < get<1>(closest_surface_right))
           ? closest_surface_left
           : closest_surface_right;
}

void BVHTree::printTree() const {
    printTree(this->root);
    cout << endl;
}

void BVHTree::printTree(BVHNode *node) const {
    if (node != NULL) {
        cout << "( ";
        printTree(node->left);
        cout << " " << node->thisBound.id << " ";
        printTree(node->right);
        cout << " )";
    }
}
