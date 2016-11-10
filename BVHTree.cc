//
// Created by bahuljain on 10/29/16.
//

#include "include/BVHTree.h"
#include <algorithm>

using namespace std;

BVHTree::BVHTree(const std::vector<Surface *> *surfaces) {
    this->root = nullptr;
    this->surfaces = surfaces;
}

BVHTree::~BVHTree() {
    delete this->root;
}

/**
 * @name    makeBVHTree
 * @brief   Given a list of surfaces it creates a Bounding Volume
 *          Hierarchical Tree structure.
 *
 * @returns  a BVHTree for the corresponding list of surfaces.
 * @see      _makeBVHTree function
 */
int BVHTree::makeBVHTree() {
    vector<BoundingBox *> bboxes;
    clock_t time;

    /*
     * A list of BoundingBox objects each corresponding to a surface.
     */
    for (unsigned int i = 0; i < surfaces->size(); i++) {
        BoundingBox *bbox = surfaces->at(i)->bbox;

        bbox->setBoundedSurface(i);
        bboxes.push_back(bbox);
    }

    cout << "Constructing BVH Tree. ";
    time = clock();
    this->root = this->_makeBVHTree(bboxes, 0, (int) (bboxes.size() - 1), 0);
    time = clock() - time;
    cout << "[Done] [" << ((float) time) / CLOCKS_PER_SEC << "s]"
         << endl << endl;

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
 * @details
 * BoundingBoxes are partially sorted at each step (based on a given
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
 */
bool BVHTree::isIntercepted(const Ray &ray, float t_max, int mode) const {
    return this->_isIntercepted(this->root, ray, t_max, mode);
}

/**
 * @name    _isIntercepted
 * @private used internally by BVHTree class (called by isIntercepted)
 * @brief   Determines if a surface intercepts the ray before it reaches it
 *          final destination.
 *
 * @param node  - the BVHTree root node.
 * @param ray   - the ray which needs to be checked if it is intercepted
 *                by the surface.
 * @param t_max - the destination of the ray; the ray should be intercepted
 *                before reaching this point; represented in terms of the
 *                parameter on the ray.
 * @param mode  - 0|-1 => check interception with leaf node surfaces.
 *                1    => check interception with leaf node bounding box.
 *
 *
 * @returns a boolean indicating if the ray was intercepted by any surface on
 *          it's way to the destination.
 */
bool BVHTree::_isIntercepted(const BVHNode *node,
                             const Ray &ray, float t_max, int mode) const {
    if (node == nullptr)
        return false;

    /*
     * If the bounding box doesn't intercepts the ray don't bother going any
     * further.
     */
    float t_bbox = node->thisBound->getIntersection(ray);
    if (t_bbox == -1)
        return false;

    /*
     * If both left and right nodes are nullptr, the node is a leaf node and we
     * need to compute intersections with the surface
     */
    if (node->left == nullptr && node->right == nullptr) {
        int surface_idx = node->thisBound->getBoundedSurface();

        if (mode == 1 && t_bbox < fabsf(t_max - 0.01f))
            return true;

        float t = surfaces->at(surface_idx)->getIntersection(ray);

        return (t >= 0 && t < fabsf(t_max - 0.01f));
    }

    /*
     * At this point we know that ray intersects thisBound, so we need to
     * traverse the left and right nodes to find intersections. If either one
     * of the node returns an interception return true.
     */
    return _isIntercepted(node->left, ray, t_max, mode) ||
           _isIntercepted(node->right, ray, t_max, mode);
}

/**
 * @name    getClosestSurface
 * @see     _getClosestSurface
 */
std::tuple<int, float>
BVHTree::getClosestSurface(const Ray &ray, int mode) const {
    auto closest = std::make_tuple(-1, numeric_limits<float>::infinity());

    return _getClosestSurface(this->root, ray, mode, closest);
}

/**
 * @name    _getClosestSurface
 * @private used internally by BVHTree class (called by getClosestSurface)
 * @brief   Returns the closest surface along the given ray
 *
 * @param node - the BVHTree root node.
 * @param ray  - the ray along which the closest surface is to be computed.
 * @param mode - 0|-1 => check interception with leaf node surfaces.
 *               1    => check interception with leaf node bounding box.
 *
 * @returns a tuple containing the index of the surface that was closest
 *          along with the intersection point of the ray and the surface or its
 *          bounding box depending on the mode.
 */
std::tuple<int, float>
BVHTree::_getClosestSurface(const BVHNode *node, const Ray &ray, int mode,
                            const std::tuple<int, float> &closest) const {
    float t_max = get<1>(closest);

    if (node == nullptr)
        return closest;

    /*
     * If bounding box doesn't intersect with ray then dont bother going
     * further and just return (-1, infinity)
     */
    float t_bbox = node->thisBound->getIntersection(ray);
    if (t_bbox == -1 || t_bbox > t_max)
        return closest;

    /*
     * If both left and right nodes are nullptr, the node is a leaf node and we
     * need to compute intersections with the actual surface. If there is an
     * actual intersection with the surface then return those values else
     * return (-1, infinity).
     */
    if (node->left == nullptr && node->right == nullptr) {
        int surface_idx = node->thisBound->getBoundedSurface();

        if (mode == 1 && t_bbox >= 0.01 && t_bbox < t_max)
            return make_tuple(surface_idx, t_bbox);

        float t = surfaces->at(surface_idx)->getIntersection(ray);

        return (t >= 0.01 && t < t_max)
               ? make_tuple(surface_idx, t) : closest;
    }

    /**
     * At this point we know the bounding box intersects the ray, so now we
     * check for intersections with the left and right nodes and get the
     * closest surface from each side. Between the two of them return the one
     * that is closer.
     */
    tuple<int, float> closest_l, closest_r;

    closest_l = this->_getClosestSurface(node->left, ray, mode, closest);
    closest_r = this->_getClosestSurface(node->right, ray, mode, closest_l);

    return closest_r;
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

bool BVHTree::isEmpty() const {
    return (this->root == nullptr);
}
