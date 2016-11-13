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
#include "BoundingBox.h"
#include <math.h>

class Surface {
private:
    Material *material;

public:
    BoundingBox *bbox;

    virtual ~Surface() {}

    virtual float getIntersection(const Ray &) const = 0;

    virtual Vector getSurfaceNormal(const Point &) const = 0;

    virtual bool isFrontFacedTo(const Ray &) const = 0;

    void setMaterial(Material *m) {
        this->material = m;
    }

    bool isReflective() const;

    RGB getDiffuseComponent() const;

    RGB getReflectiveComponent() const;

    RGB getSpecularComponent() const;

    RGB phongShading(const PointLight *light,
                     const Ray &light_ray,
                     const Ray &view_ray,
                     const Point &intersection,
                     int mode) const;
};


#endif //RAYTRA_SURFACE_H
