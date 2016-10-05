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
    Material m;
public:
    virtual int getIntersection(const Ray &) const = 0;

    void setMaterial(const Material & m) {
        this->m = m;
    }

    Material getMaterial() {
        return m;
    }
};


#endif //RAYTRA_SURFACE_H
