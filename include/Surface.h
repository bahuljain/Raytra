//
// Created by bahuljain on 10/4/16.
//

#ifndef RAYTRA_SURFACE_H
#define RAYTRA_SURFACE_H

#include <iostream>
#include "Point.h"
#include "Ray.h"
#include "Material.h"
#include "Light.h"
#include <math.h>

class Surface {
private:
    Material *material;
public:
    virtual ~Surface() {}

    virtual float getIntersection(const Ray &) const = 0;

    virtual Vector getSurfaceNormal(const Point &) const = 0;

    virtual bool isFrontFacedTo(const Vector &) const = 0;

    void setMaterial(Material *m) {
        this->material = m;
    }

    bool isReflective() const;

    RGB getDiffuseComponent() const;
    RGB getReflectiveComponent() const;
    RGB getSpecularComponent() const;

    RGB phongShading(const Light *,
                     const Ray &,
                     const Ray &,
                     const Point &) const;
};


#endif //RAYTRA_SURFACE_H
