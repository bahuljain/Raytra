/**
 * @file    Surface.cc
 * @author  Bahul Jain
 * @date    10/28/16
 * @brief   Holds all constructors and members of the Bounding Box class.
 */

#include <limits>
#include <math.h>
#include <iostream>
#include "include/BoundingBox.h"

BoundingBox::BoundingBox() {
    this->id = -1;
    this->x_min = 0;
    this->x_max = 0;
    this->y_min = 0;
    this->y_max = 0;
    this->z_min = 0;
    this->z_max = 0;

    this->setBoundedSurface(-1);

    center = Point();
}

BoundingBox::BoundingBox(float x_min, float x_max,
                         float y_min, float y_max,
                         float z_min, float z_max) {

    this->id = rand() % 10000;

    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
    this->z_min = z_min;
    this->z_max = z_max;

    /* Need to make adjustments if bbox is completely flat */
    if (x_min == x_max) this->x_min -= 0.01;
    if (y_min == y_max) this->y_min -= 0.01;
    if (z_min == z_max) this->z_min -= 0.01;

    this->setBoundedSurface(-1);

    center = Point((x_min + x_max) / 2,
                   (y_min + y_max) / 2,
                   (z_min + z_max) / 2);
}

void BoundingBox::setBoundedSurface(int index) {
    this->bounded_surface_idx = index;
}

int BoundingBox::getBoundedSurface() const {
    return this->bounded_surface_idx;
}


/**
 * @name    groupBoundingBoxes
 * @brief   Forms a bounding box that engulfs a group of given bounding boxes.
 *
 * @param bboxes - a vector of bounding boxes of which a part of them need to
 *                 be grouped into a single bounding box.
 * @param start  - the index in the above list representing the starting box
 * @param end    - the index in the above list representing the last box
 * @returns        A bounding box formed after grouping all the given
 *                 bounding boxes.
 */
BoundingBox *BoundingBox::groupBoundingBoxes(std::vector<BoundingBox *> &bboxes,
                                             int start, int end) {
    float x_min, x_max, y_min, y_max, z_min, z_max;

    x_min = y_min = z_min = std::numeric_limits<float>::infinity();
    x_max = y_max = z_max = -std::numeric_limits<float>::infinity();

    for (int i = start; i <= end; i++) {
        x_min = fminf(x_min, bboxes[i]->x_min);
        y_min = fminf(y_min, bboxes[i]->y_min);
        z_min = fminf(z_min, bboxes[i]->z_min);

        x_max = fmaxf(x_max, bboxes[i]->x_max);
        y_max = fmaxf(y_max, bboxes[i]->y_max);
        z_max = fmaxf(z_max, bboxes[i]->z_max);
    }

    return new BoundingBox(x_min, x_max, y_min, y_max, z_min, z_max);
}

/**
 * @name    compare
 * @brief   returns a comparision function for two BoundingBoxes given the
 *          axis along which comparison is to be done.
 *
 * @param axis - the axis along which comparision needs to be done
 *              0 - x-axis
 *              1 - y-axis
 *              2 - z-axis
 *
 * @returns a lambda function that takes two BoundingBoxes and returns a
 *          boolean value indicating which BoundingBox is appears first along
 *          the given axis.
 */
std::function<bool(BoundingBox *, BoundingBox *)>
BoundingBox::compare(int axis) {

    if (axis == 2) {
        return [](BoundingBox *a, BoundingBox *b) -> bool {
            return a->center.z < b->center.z;
        };
    } else if (axis == 1) {
        return [](BoundingBox *a, BoundingBox *b) -> bool {
            return a->center.y < b->center.y;
        };
    } else {
        return [](BoundingBox *a, BoundingBox *b) -> bool {
            return a->center.x < b->center.x;
        };
    }
}

/**
 * @name    printBox
 * @brief   prints the min and max values of the box along each axis.
 * @note    used only for debugging purposes.
 */
void BoundingBox::printBox() const {
    std::cout << id << ": "
              << "(" << x_min << ", " << x_max << ") "
              << "(" << y_min << ", " << y_max << ") "
              << "(" << z_min << ", " << z_max << ") " << std::endl;
}

/**
 * @name    getSurfaceNormal
 * @brief   returns the normal on surface of the bounding box.
 *
 * @param p - the point on the surface at which the normal has to be computed.
 * @attention @param p has to lie on the surface else shit will go down.
 *
 * @returns normal vector to the surface at the given point.
 *
 * @note at times due to rounding errors the point might slightly be inside
 * or outside the bounding box. Such cases need to be handled.
 */
Vector BoundingBox::getSurfaceNormal(const Point &p) const {
    auto approx_eq = [](float x, float y) -> bool {
        return fabsf(x - y) <= 0.001;
    };

    if (approx_eq(p.x, x_min)) return Vector(-1, 0, 0);
    if (approx_eq(p.x, x_max)) return Vector(1, 0, 0);
    if (approx_eq(p.y, y_min)) return Vector(0, -1, 0);
    if (approx_eq(p.y, y_max)) return Vector(0, 1, 0);
    if (approx_eq(p.z, z_min)) return Vector(0, 0, -1);
    if (approx_eq(p.z, z_max)) return Vector(0, 0, 1);

    return Vector(0, 0, -1);
}

/**
 * @name    getIntersection
 * @brief   determines the intersection point of a ray with the bounding box.
 *
 * @param ray    - the ray with which intersection needs to be checked.
 * @returns      - the parameterized location of the intersection point on
 *                 the ray.
 * @retval   -1  - if the ray doesn't intersect with the bounding box.
 *
 * @note - remember the case when ray is parallel to any of the axes.
 * @note - if ray direction along any axis is negative the max side will be
 *         struck first on that axis. In such a case the intersection points
 *         corresponding to the min and max planes should be swapped with
 *         each other.
 */
float BoundingBox::getIntersection(const Ray &ray) const {
    float t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max;
    float t_min = 0;
    float t_max = std::numeric_limits<float>::infinity();

    if (ray.direction.i == 0 && (ray.origin.x < x_min || ray.origin.x > x_max))
        return -1;

    t_x_min = (x_min - ray.origin.x) / ray.direction.i;
    t_x_max = (x_max - ray.origin.x) / ray.direction.i;

    if (ray.direction.i < 0) std::swap(t_x_min, t_x_max);

    t_min = fmaxf(t_min, t_x_min);
    t_max = fminf(t_max, t_x_max);

    if (ray.direction.j == 0 && (ray.origin.y < y_min || ray.origin.y > y_max))
        return -1;

    t_y_min = (y_min - ray.origin.y) / ray.direction.j;
    t_y_max = (y_max - ray.origin.y) / ray.direction.j;

    if (ray.direction.j < 0) std::swap(t_y_min, t_y_max);

    if (t_y_min > t_max) return -1;

    t_min = fmaxf(t_min, t_y_min);
    t_max = fminf(t_max, t_y_max);

    if (ray.direction.k == 0 && (ray.origin.z < z_min || ray.origin.z > z_max))
        return -1;

    t_z_min = (z_min - ray.origin.z) / ray.direction.k;
    t_z_max = (z_max - ray.origin.z) / ray.direction.k;

    if (ray.direction.k < 0) std::swap(t_z_min, t_z_max);

    t_min = fmaxf(t_min, t_z_min);
    t_max = fminf(t_max, t_z_max);

    return (t_min > t_max) ? -1 : t_min;
}
