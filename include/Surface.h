//
// Created by bahuljain on 10/4/16.
//

#ifndef RAYTRA_SURFACE_H
#define RAYTRA_SURFACE_H


#include "Point.h"
#include "Ray.h"
#include "Material.h"
#include <math.h>

class Surface {
private:
    Material *material;
public:
    virtual ~Surface() {}

    virtual float getIntersection(const Ray &) const = 0;
    virtual Vector getSurfaceNormal(const Point &) const = 0;

    bool intercepts(const Ray &ray, float t_max) {
        float t = this->getIntersection(ray);

        if (fabsf(t - t_max) <= 0.01)
            t = -1;

        return (t >= 0 && t < t_max);
    }

    void setMaterial(Material *m) {
        this->material = m;
    }

    Material *getMaterial() const {
        return this->material;
    }
};


#endif //RAYTRA_SURFACE_H
