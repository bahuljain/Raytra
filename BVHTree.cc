//
// Created by bahuljain on 10/29/16.
//

#include "include/BVHTree.h"
#include <algorithm>

using namespace std;

BVHTree::BVHTree() {
    this->root = nullptr;
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
 * @see     _makeBVHTree function
 */
int BVHTree::makeBVHTree(const vector<Surface *> &surfaces) {
    vector<BoundingBox *> bboxes;

    cout << "Constructing a bounding box for each surface. ";

    /*
     * A list of BoundingBox objects each corresponding to a surface.
     */
    for (unsigned int i = 0; i < surfaces.size(); i++) {
        BoundingBox *bbox = surfaces[i]->bbox;

        bbox->setBoundedSurface(i);
        bboxes.push_back(bbox);
    }

    cout << "[Done]" << endl << "Constructing BVH Tree. ";

    this->root = this->_makeBVHTree(bboxes, 0, (int) (bboxes.size() - 1), 0);

    cout << "[Done]" << endl;

    return (this->root != nullptr);

}

/**
 * @name    _makeBVHTree
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
BVHNode *BVHTree::_makeBVHTree(vector<BoundingBox *> &bboxes,
                               int start, int end, int axis) const {
    BVHNode *node;

    if (start > end) {
        cout << "Should never reach here" << endl;
        return nullptr;
    }

    node = new BVHNode();

    /**
     * If only one bounding box remains just make that bounding box as
     * thisBound for @var node. Left and right pointers will remain nullptr.
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
    int mid = start + (end - start) / 2;

    nth_element(bboxes.begin() + start,
                bboxes.begin() + mid,
                bboxes.begin() + end,
                BoundingBox::compare(axis));

    /*
     * Recursively create tree for the left and right partitions from the
     * middle element.
     */
    node->left = this->_makeBVHTree(bboxes, start, mid, (axis + 1) % 3);
    node->right = this->_makeBVHTree(bboxes, mid + 1, end, (axis + 1) % 3);

    return node;
}

/**
 * @name    isIntercepted
 * @see     _isIntercepted
 * @returns a boolean indicating if the ray was intercepted by any surface on
 *          it's way to the destination.
 */
bool BVHTree::isIntercepted(const Ray &ray,
                            const std::vector<Surface *> &surfaces,
                            float t_max,
                            int origin_surface_idx,
                            int mode) const {
    return this->_isIntercepted(this->root,
                                surfaces, ray, t_max,
                                origin_surface_idx,
                                mode);
}

/**
 * @name    _isIntercepted
 * @private used internally by BVHTree class (called by inIntercepted)
 * @brief   Determines if a surface intercepts the ray before it reaches it
 *          final destination.
 *
 * @param node  - the BVHTree root node.
 * @param surfaces - list of all surfaces in the scene.
 * @param ray   - the ray which needs to be checked if it is intercepted
 *                by the surface.
 * @param t_max - the destination of the ray; the ray should be intercepted
 *                before reaching this point; represented in terms of the
 *                parameter on the ray.
 *
 * @retval TRUE  - A surface intercepts the ray before reaching its destination.
 * @retval FALSE - A surface doesn't intercept the ray before reaching its
 *                 destination.
 */
bool BVHTree::_isIntercepted(const BVHNode *node,
                             const vector<Surface *> &surfaces,
                             const Ray &ray,
                             float t_max,
                             int origin_surface_idx,
                             int mode) const {
    if (node == nullptr)
        return false;

    /*
     * If the bounding box doesn't intercepts the ray don't bother going any
     * further.
     */
    if (node->thisBound->getIntersection(ray) == -1)
        return false;

    /*
     * If both left and right nodes are nullptr, the node is a leaf node and we
     * need to compute intersections with the surface
     */
    if (node->left == nullptr && node->right == nullptr) {
        int surface_idx = node->thisBound->getBoundedSurface();

        if (surface_idx == origin_surface_idx)
            return false;

        if (mode == 1)
            return true;

        float t = surfaces[surface_idx]->getIntersection(ray);

        return (t >= 0 && t < t_max);
    }

    /*
     * At this point we know that ray intersects thisBound, so we need to
     * traverse the left and right nodes to find intersections. If either one
     * of the node returns an interception return true.
     */
    return _isIntercepted(node->left, surfaces, ray,
                          t_max, origin_surface_idx, mode) ||
           _isIntercepted(node->right, surfaces, ray,
                          t_max, origin_surface_idx, mode);
}

/**
 * @name    getClosestSurface
 * @see     _getClosestSurface
 */
std::tuple<int, float>
BVHTree::getClosestSurface(const std::vector<Surface *> &surfaces,
                           const Ray &ray,
                           int origin_surface_idx,
                           int mode) const {
    return _getClosestSurface(this->root, surfaces, ray,
                              origin_surface_idx, mode);
}

/**
 *
 */
tuple<int, float>
BVHTree::_getClosestSurface(const BVHNode *node,
                            const vector<Surface *> &surfaces,
                            const Ray &ray,
                            int origin_surface_idx,
                            int mode) const {
    tuple<int, float> invalid =
            make_tuple(-1, numeric_limits<float>::infinity());

    if (node == nullptr)
        return invalid;

    /*
     * If bounding box doesn't intersect with ray then dont bother going
     * further and just return (-1, infinity)
     */
    float t_bbox = node->thisBound->getIntersection(ray);
    if (t_bbox == -1)
        return invalid;

    /*
     * If both left and right nodes are nullptr, the node is a leaf node and we
     * need to compute intersections with the actual surface. If there is an
     * actual intersection with the surface then return those values else
     * return (-1, infinity).
     */
    if (node->left == nullptr && node->right == nullptr) {
        int surface_idx = node->thisBound->getBoundedSurface();

        if (surface_idx == origin_surface_idx || surface_idx == -1)
            return invalid;

        if (mode == 1)
            return make_tuple(surface_idx, t_bbox);

        float t = surfaces[surface_idx]->getIntersection(ray);

        return (t >= 0)
               ? make_tuple(surface_idx, t)
               : invalid;
    }

    /**
     * At this point we know the bounding box intersects the ray, so now we
     * check for intersections with the left and right nodes and get the
     * closest surface from each side. Between the two of them return the one
     * that is closer.
     */
    tuple<int, float> closest_surface_left, closest_surface_right;

    closest_surface_left =
            this->_getClosestSurface(node->left, surfaces,
                                     ray, origin_surface_idx, mode);

    closest_surface_right =
            this->_getClosestSurface(node->right, surfaces,
                                     ray, origin_surface_idx, mode);

    return (get<1>(closest_surface_left) < get<1>(closest_surface_right))
           ? closest_surface_left
           : closest_surface_right;
}

void BVHTree::printTree() const {
    printTree(this->root);
    cout << endl;
}

void BVHTree::printTree(BVHNode *node) const {
    if (node != nullptr) {
        cout << "( ";
        printTree(node->left);
        cout << " " << node->thisBound->id << " ";
        printTree(node->right);
        cout << " )";
    }
}

int BVHTree::getMaxHeight() {
    return this->_getMaxHeight(this->root);
}

int BVHTree::_getMaxHeight(BVHNode *node) const {
    if (node == nullptr)
        return 0;

//    node->thisBound->printBox();
    return 1 + max(this->_getMaxHeight(node->left),
                   this->_getMaxHeight(node->right));
}


void BVHTree::intercepts(const Ray &ray,
                         std::vector<int> &intersection_indices) const {
    this->_intercepts(this->root, ray, intersection_indices);
}

void BVHTree::_intercepts(const BVHNode *node,
                          const Ray &ray,
                          std::vector<int> &intersection_indices) const {
    if (node == nullptr)
        return;

    if (node->thisBound->getIntersection(ray) == -1)
        return;

    if (node->left == nullptr && node->right == nullptr) {
        intersection_indices.push_back(node->thisBound->getBoundedSurface());
        return;
    }

    this->_intercepts(node->left, ray, intersection_indices);
    this->_intercepts(node->right, ray, intersection_indices);
}
