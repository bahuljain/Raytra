//
// Created by bahuljain on 10/23/16.
//

#include "include/Surface.h"

bool Surface::intercepts(const Ray &ray, float t_max) const {
    float t = this->getIntersection(ray);

    if (fabsf(t - t_max) <= 0.01)
        t = -1;

    return (t >= 0 && t < t_max);
}

RGB Surface::phongShading(const Light *light,
                          const Ray &light_ray,
                          const Ray &view_ray,
                          const Point &intersection) const {

    float r, g, b, d2, diffuse_factor, specular_factor;
    Vector v, normal, I, bisector;

    /* Material not defined for the surface */
    if (!material) {
        std::cout << "Material not defined for the surface!" << std::endl;
        return RGB(0, 0, 0);
    }

    /* Vector to the viewer */
    v = -view_ray.direction;

    if (!this->isFrontFaced(view_ray)) {
        std::cout << "Surface not oriented correctly!" << std::endl;
        return RGB(1, 1, 0);
    }

    /* Distance of light from the point of intersection */
    d2 = light->position.distance2(intersection);

    /* Accounting for zero distance, in which case there shouldn't be any distance attenuation */
    if (d2 == 0)
        d2 = 1;

    /* Vector normal to the surface at the given intersection point */
    normal = this->getSurfaceNormal(intersection);

    /* Vector to the light source */
    I = -light_ray.direction;

    /* Vector bisecting the view vector and light vector */
    bisector = v.plus(I).norm();

    diffuse_factor = light->intensity * fmaxf(0, normal.dot(I));

    specular_factor = light->intensity *
                      powf(fmaxf(0, normal.dot(bisector)), material->phong);

    r = (material->diffuse.r * diffuse_factor
         + material->specular.r * specular_factor) * light->color.r / d2;

    g = (material->diffuse.g * diffuse_factor
         + material->specular.g * specular_factor) * light->color.g / d2;

    b = (material->diffuse.b * diffuse_factor
         + material->specular.b * specular_factor) * light->color.b / d2;

    return RGB(r, g, b);
}