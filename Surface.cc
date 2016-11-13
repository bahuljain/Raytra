/**
 * @file    Surface.cc
 * @author  Bahul Jain
 * @date    10/23/16
 * @brief   Holds all constructors and members of the Surface class.
 */

#include "include/Surface.h"


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
RGB Surface::phongShading(const PointLight *light,
                          const Ray &light_ray,
                          const Ray &view_ray,
                          const Point &intersection,
                          int mode) const {

    float r, g, b, d2, diffuse_factor, specular_factor;
    Vector v, normal, I, bisector;
    RGB material_diffuse, material_specular;

    /* Material not defined for the surface */
    if (!material) {
        std::cout << "Material not defined for the surface!" << std::endl;
        return RGB(0, 0, 0);
    }

    /* Vector normal to the surface at the given intersection point */
    normal = (mode == 1)
             ? this->bbox->getSurfaceNormal(intersection)
             : this->getSurfaceNormal(intersection);

    /*
     * TODO: rethink if necessary to do it in this particular way.
     * If the light hits the back side of the surface then simply give it a
     * unique diffuse and specular values and invert the surface normal.
     */
    if (mode == 1 || this->isFrontFacedTo(view_ray)) {
        material_diffuse = material->diffuse;
        material_specular = material->specular;
    } else {
        material_diffuse = RGB(1, 1, 0);
        material_specular = RGB(0, 0, 0);
        normal = -normal;
    }

    /* Vector to the viewer */
    v = -view_ray.direction;

    /*
     * Distance of light from the point of intersection squared.
     * Note: distance cannot be less than 1 else intensity values will get
     * scaled up which shouldn't happen.
     */
    d2 = fmaxf(1, light_ray.origin.distance2(intersection));

    /* Vector to the light source */
    I = -light_ray.direction;

    /* Vector bisecting the view vector and light vector */
    bisector = v.plus(I).norm();

    diffuse_factor = light->intensity * fmaxf(0, normal.dot(I));

    specular_factor = light->intensity *
                      powf(fmaxf(0, normal.dot(bisector)), material->phong);

    r = (material_diffuse.r * diffuse_factor
         + material_specular.r * specular_factor) * light->color.r / d2;

    g = (material_diffuse.g * diffuse_factor
         + material_specular.g * specular_factor) * light->color.g / d2;

    b = (material_diffuse.b * diffuse_factor
         + material_specular.b * specular_factor) * light->color.b / d2;

    return RGB(r, g, b);
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
RGB Surface::phongShading2(const SquareLight *light,
                          const Ray &light_ray,
                          const Ray &view_ray,
                          const Point &intersection,
                          int mode) const {

    float r, g, b, d2, diffuse_factor, specular_factor;
    Vector v, normal, I, bisector;
    RGB material_diffuse, material_specular, light_color;

    /* Material not defined for the surface */
    if (!material) {
        std::cout << "Material not defined for the surface!" << std::endl;
        return RGB(0, 0, 0);
    }

    /* Vector normal to the surface at the given intersection point */
    normal = (mode == 1)
             ? this->bbox->getSurfaceNormal(intersection)
             : this->getSurfaceNormal(intersection);

    /*
     * TODO: rethink if necessary to do it in this particular way.
     * If the light hits the back side of the surface then simply give it a
     * unique diffuse and specular values and invert the surface normal.
     */
    if (mode == 1 || this->isFrontFacedTo(view_ray)) {
        material_diffuse = material->diffuse;
        material_specular = material->specular;
    } else {
        material_diffuse = RGB(1, 1, 0);
        material_specular = RGB(0, 0, 0);
        normal = -normal;
    }

    float _factor = fmaxf(0, light_ray.direction.dot(light->w));
    light_color = light->color.scaleRGB(RGB(_factor, _factor, _factor));

    /* Vector to the viewer */
    v = -view_ray.direction;

    /*
     * Distance of light from the point of intersection squared.
     * Note: distance cannot be less than 1 else intensity values will get
     * scaled up which shouldn't happen.
     */
    d2 = fmaxf(1, light_ray.origin.distance2(intersection));

    /* Vector to the light source */
    I = -light_ray.direction;

    /* Vector bisecting the view vector and light vector */
    bisector = v.plus(I).norm();

    diffuse_factor = light->intensity * fmaxf(0, normal.dot(I));

    specular_factor = light->intensity *
                      powf(fmaxf(0, normal.dot(bisector)), material->phong);

    r = (material_diffuse.r * diffuse_factor
         + material_specular.r * specular_factor) * light_color.r / d2;

    g = (material_diffuse.g * diffuse_factor
         + material_specular.g * specular_factor) * light_color.g / d2;

    b = (material_diffuse.b * diffuse_factor
         + material_specular.b * specular_factor) * light_color.b / d2;

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
