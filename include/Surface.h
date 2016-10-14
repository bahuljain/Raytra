//
// Created by bahuljain on 10/4/16.
//

#ifndef RAYTRA_SURFACE_H
#define RAYTRA_SURFACE_H


#include "Point.h"
#include "Ray.h"
#include "Material.h"

class Surface {
private:
    Material *material;
public:
    virtual ~Surface() {}

    virtual float getIntersection(const Ray &) const = 0;

    virtual Vector getSurfaceNormal(const Point &) const = 0;

    void setMaterial(Material *m) {
        this->material = m;
    }

    Material *getMaterial() {
        return this->material;
    }
};


#endif //RAYTRA_SURFACE_H
