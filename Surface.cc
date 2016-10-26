/**
 * @file    Surface.cc
 * @author  Bahul Jain
 * @date    10/23/16
 * @brief   Holds all constructors and members of the Surface class.
 */

#include "include/Surface.h"


/**
 * @name    intercepts
 * @brief   Determines if the surface intercepts the ray before it reaches it
 *          final destination.
 *
 * @param ray   - the ray which needs to be checked if it is intercepted
 *                by the surface.
 * @param t_max - the destination of the ray; the ray should be intercepted
 *                before reaching this point; represented in terms of the
 *                parameter on the ray.
 *
 * @retval TRUE  - Surface intercepts the ray before reaching its destination.
 * @retval FALSE - Surface doesn't intercept the ray before reaching its
 *                 destination.
 */
bool Surface::intercepts(const Ray &ray, float t_max) const {
    float t = this->getIntersection(ray);

    if (fabsf(t - t_max) <= 0.01)
        t = -1;

    return (t >= 0 && t < t_max);
}



/**
 * @name    phongShading
 * @brief   Determines the shade on the surface at a given point on it.
 *
 * @param light - the light source used for shading.
 * @param light_ray - a ray originating from the light source to the surface.
 * @param view_ray - a ray originating from the camera to the surface.
 * @param intersection - the point on the surface where the view ray intersects
                       it.
 * @returns - an RGB object representing the shade on the surface determined
            using phong's shading model.
 */
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

    /*if (!this->isFrontFaced(view_ray)) {
        std::cout << "Surface not oriented correctly!" << std::endl;
        return RGB(1, 1, 0);
    }*/

    /* Distance of light from the point of intersection */
    d2 = light->position.distance2(intersection);

    /*
     * Accounting for zero distance, in which case there shouldn't
     * be any distance attenuation.
     */
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


/**
 * @returns - a boolean indicating whether the surface is reflective or not.
 */
bool Surface::isReflective() const {
    return material->ideal_specular.r > 0 ||
           material->ideal_specular.g > 0 ||
           material->ideal_specular.b > 0;
}


/**
 * @returns the diffuse coefficients of the material on the surface.
 */
RGB Surface::getDiffuseComponent() const {
    return material->diffuse;
}


/**
 * @returns the ideal specular coefficients of the material on the surface.
 */
RGB Surface::getReflectiveComponent() const {
    return material->ideal_specular;
}


/**
 * @returns the specular coefficients of the material on the surface.
 */
RGB Surface::getSpecularComponent() const {
    return material->specular;
}
