/**
 * @file    Surface.cc
 * @author  Bahul Jain
 * @date    10/28/16
 * @brief   Holds all constructors and members of the Bounding Box class.
 */

#include <limits>
#include <math.h>
#include "include/BoundingBox.h"

BoundingBox::BoundingBox() {
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
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
    this->z_min = z_min;
    this->z_max = z_max;

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
 * @name intersects
 * @brief determines if a ray intersects with a bounding box or no.
 *
 * @param ray    - the ray with which intersection needs to be checked.
 * @retval TRUE  - when ray intersects with bounding box.
 * @retval FALSE - when ray doesn't intersect with bounding box.
 *
 * @note - remember the case when ray is parallel to any of the axes.
 * @note - if ray direction along any axis is negative the max side will be
 *         struck first on that axis. In such a case the intersection points
 *         corresponding to the min and max planes should be swapped with
 *         each other.
 */
bool BoundingBox::intersects(const Ray &ray) const {
    float t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max;
    float max = std::numeric_limits<float>::infinity();

    if (ray.direction.i == 0 && (ray.origin.x < x_min || ray.origin.x > x_max))
        return false;

    t_x_min = (x_min - ray.origin.x) / ray.direction.i;
    t_x_max = (x_max - ray.origin.x) / ray.direction.i;

    if (ray.direction.i < 0)
        std::swap(t_x_min, t_x_max);

    max = fminf(max, t_x_max);

    if (ray.direction.j == 0 && (ray.origin.y < y_min || ray.origin.y > y_max))
        return false;

    t_y_min = (y_min - ray.origin.y) / ray.direction.j;
    t_y_max = (y_max - ray.origin.y) / ray.direction.j;

    if (ray.direction.j < 0)
        std::swap(t_y_min, t_y_max);

    if (t_y_min > max)
        return false;

    max = fminf(max, t_y_max);

    if (ray.direction.k == 0 && (ray.origin.z < z_min || ray.origin.z > z_max))
        return false;

    t_z_min = (z_min - ray.origin.z) / ray.direction.k;
    t_z_max = (z_max - ray.origin.z) / ray.direction.k;

    if (ray.direction.k < 0)
        std::swap(t_z_min, t_z_max);

    return max >= t_z_min;

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
BoundingBox BoundingBox::groupBoundingBoxes(std::vector<BoundingBox> &bboxes,
                                            int start, int end) {
    float x_min, x_max, y_min, y_max, z_min, z_max;

    x_min = y_min = z_min = std::numeric_limits<float>::infinity();
    x_max = y_max = z_max = -std::numeric_limits<float>::infinity();

    for (int i = start; i <= end; i++) {
        x_min = fminf(x_min, bboxes[i].x_min);
        y_min = fminf(y_min, bboxes[i].y_min);
        z_min = fminf(z_min, bboxes[i].z_min);

        x_max = fmaxf(x_max, bboxes[i].x_max);
        y_max = fmaxf(y_max, bboxes[i].y_max);
        z_max = fmaxf(z_max, bboxes[i].z_max);
    }

    return BoundingBox(x_min, x_max, y_min, y_max, z_min, z_max);
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
std::function<bool(const BoundingBox &, const BoundingBox &)>
BoundingBox::compare(int axis) {

    if (axis == 2) {
        return [](const BoundingBox &a, const BoundingBox &b) {
            return a.z_min < b.z_min;
        };
    } else if (axis == 1) {
        return [](const BoundingBox &a, const BoundingBox &b) {
            return a.y_min < b.y_min;
        };
    } else {
        return [](const BoundingBox &a, const BoundingBox &b) {
            return a.x_min < b.x_min;
        };
    }
}

