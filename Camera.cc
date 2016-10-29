/**
 * @file    Camera.cc
 * @author  Bahul Jain
 * @date    10/3/16
 * @brief   Holds all constructors and members of the Camera class.
 *
 */

#include <tuple>
#include <limits>
#include "include/Camera.h"
#include "include/ProgressBar.h"

using namespace Imf;
using namespace std;

Camera::Camera() {
    this->eye = Point(0, 0, 0);

    /* Camera is pointing straight ahead*/
    Vector D = Vector(0.0, 0.0, -1.0f);

    this->w = -D;

    /* Note here D should not be [0 t 0] */
    this->u = Vector(1, 0, 0);
    this->v = Vector(0, 1, 0);

    this->d = 1;

    this->right = 0;
    this->left = 0;
    this->top = 0;
    this->bottom = 0;

    this->pw = 0;
    this->ph = 0;
}

void Camera::setValues(float x, float y, float z,
                       float vx, float vy, float vz, float d,
                       float iw, float ih, int pw, int ph) {

    this->eye = Point(x, y, z);

    /* Direction the camera is pointing to */
    Vector D = Vector(vx, vy, vz).norm();

    this->w = -D;

    /* Note here D should not be [0 t 0] */
    this->u = D.cross(Vector(0, 1, 0)).norm();
    this->v = D.cross(u).norm();

    this->d = d;

    this->right = iw / 2;
    this->left = -right;
    this->top = ih / 2;
    this->bottom = -top;

    this->pw = pw;
    this->ph = ph;
}

/**
 * @name    getPixelCenter
 * @brief   Finds the co-ordinates of center of the given pixel.
 *
 * @param i         - the row index of the pixel
 * @param j         - the column index of the pixel
 * @param width     - the width of the image plane
 * @param height    - the height of the image plane
 * @returns         - the pixel center co-ordinates for the given pixel
 */
Point Camera::getPixelCenter(int i, int j,
                             float width, float height) const {
    Point center;

    float x = left + width * (i + 0.5f) / pw;
    float y = bottom + height * (j + 0.5f) / ph;

    center = eye
            .moveAlong(u.times(x))
            .moveAlong(v.times(y))
            .moveAlong(w.times(-d));

    return center;
}

/**
 * @name    getClosestSurface
 * @brief   Finds the closest surface to the camera along a given ray.
 *
 * @param surfaces       - a vector of all the surfaces in the scene.
 * @param ray            - a ray originating from the camera and passing
 *                         through the pixel center of the image.
 * @param origin_surface - the surface from which the ray has originated and
 *                         therefore needs to be ignored if an intersection is
 *                         found with it.
 *
 * @returns        - a tuple containing the index of the closest intersecting
 *                   surface and the parameter representing the intersection
 *                   point on the view ray.
 */
tuple<int, float> Camera::getClosestSurface(
        const vector<Surface *> &surfaces,
        const Ray &ray,
        int origin_surface) const {

    float min_t = numeric_limits<float>::infinity();
    int min_i = -1;

    for (unsigned int i = 0; i < surfaces.size(); i++) {
        if ((int) i != origin_surface) {
            float t = surfaces[i]->getIntersection(ray);

            if (t >= 0 && t < min_t) {
                min_t = t;
                min_i = i;
            }
        }
    }
    return make_tuple(min_i, min_t);
}

/**
 * @name    isIntercepted
 * @brief   Determines if a surface intercepts the ray before it reaches it
 *          final destination.
 *
 * @param ray   - the ray which needs to be checked if it is intercepted
 *                by the surface.
 * @param t_max - the destination of the ray; the ray should be intercepted
 *                before reaching this point; represented in terms of the
 *                parameter on the ray.
 *
 * @retval TRUE  - A surface intercepts the ray before reaching its destination.
 * @retval FALSE - A surface doesn't intercept the ray before reaching its
 *                 destination.
 */
bool Camera::isIntercepted(const vector<Surface *> &surfaces,
                           const Ray &ray,
                           float t_max,
                           int origin_surface) const {
    for (unsigned int i = 0; i < surfaces.size(); i++) {
        if ((int) i != origin_surface) {
            float t = surfaces[i]->getIntersection(ray);

            if (t >= 0 && t < t_max)
                return true;
        }
    }
    return false;
}

/**
 * @name    shadeAlongRay
 * @brief   Computes the shading along the given view ray
 *
 * @param view_ray    - the ray along which shading needs to be computed.
 * @param surfaces    - a list of all the surfaces in the scene.
 * @param lights      - a list of all the light sources in the scene.
 * @param refl_limit  - the number of reflections allowed before the light
 *                      fades away.
 * @param origin_surface_idx - the index of the surface from which the ray is
 *                      coming from. Set to any negative value if ray is coming
 *                      from the viewer and not some surface.
 * @returns           - the RGB value (spectral distribution) obtained along
 *                      the given view ray
 */
RGB Camera::shadeAlongRay(const Ray &view_ray,
                          const vector<Surface *> &surfaces,
                          const vector<Light *> &lights,
                          int refl_limit, int origin_surface_idx) const {
    RGB shade(0, 0, 0);

    /*
     * If the number of reflections exceed beyond a limit there shouldn't be
     * any more light returned.
     */
    if (refl_limit == 0)
        return shade;

    /* Get closest surface along the ray */
    tuple<int, float> closest_surface =
            this->getClosestSurface(surfaces, view_ray, origin_surface_idx);

    int closest_surface_idx = get<0>(closest_surface);
    float t = get<1>(closest_surface);


    if (closest_surface_idx != -1) {
        Point intersection = view_ray.getPointOnIt(t);
        Surface *surface = surfaces[closest_surface_idx];

        for (Light *light : lights) {
            /*
             * If the light source is an ambient light then simply add it to the
             * shading.
             * Note: Only add ambient light if computing shading for view ray
             * and not reflected/refracted rays.
             */
            if (light->getType() == 'a' && origin_surface_idx == -1) {
                shade.addRGB(surface->getDiffuseComponent()
                                     .scaleRGB(light->color));
                continue;
            }

            /*
             * A light ray going from the light source to the point of
             * intersection on the surface.
             */
            Ray light_ray(light->position,
                          intersection.sub(light->position).norm());

            /*
             * The parameterized representation of the intersection point with
             * the surface on the light ray.
             */
            float t_max = light_ray.getOffsetFromOrigin(intersection);

            /*
             * If a light ray is not intercepted by another surface on its way
             * to the intersection point then compute the diffuse and specular
             * shading on the surface.
             */
            if (!isIntercepted(surfaces, light_ray, t_max,
                               closest_surface_idx))
                shade.addRGB(surface->phongShading(light, light_ray, view_ray,
                                                   intersection));
        }

        /*
         * If the surface is reflective and is front-faced with respect to
         * the view ray then compute shading from the reflected ray.
         */
        if (surface->isReflective() &&
                surface->isFrontFacedTo(view_ray)) {
            Vector normal = surface->getSurfaceNormal(intersection);

            /*
             * Directional vector generated when the view_ray reflects off the
             * surface.
             */
            Vector reflected_vector = view_ray.direction
                    .plus(-normal.times(2 * view_ray.direction.dot(normal)))
                    .norm();

            Ray reflected_ray(intersection, reflected_vector);

            /* The shade obtained from the ray that reflected off the surface. */
            RGB reflection = this->shadeAlongRay(reflected_ray, surfaces,
                                                 lights, refl_limit - 1,
                                                 closest_surface_idx);

            shade.addRGB(
                    reflection.scaleRGB(surface->getReflectiveComponent()));
        }
    }

    return shade;
}

/**
 * @name    render
 * @brief   Renders the image using the ray tracing algorithm.
 *
 * @param pixels    - a two dimensional array of pixels representing the image.
 * @param surfaces  - a vector of all the surfaces in the scene.
 * @param materials - a vector of all the materials used.
 * @param lights    - a vector of all the lights in the scene.
 */
void Camera::render(Array2D <Rgba> &pixels,
                    const vector<Surface *> &surfaces,
                    const vector<Material *> &materials,
                    const vector<Light *> &lights) const {

    ProgressBar progress = ProgressBar();
    progress.start();

    float w = this->right - this->left;
    float h = this->top - this->bottom;
    float total_pixels = this->ph * this->pw;

    pixels.resizeErase(this->ph, this->pw);

    for (int i = 0; i < this->ph; i++) {
        for (int j = 0; j < this->pw; j++) {

            Point px_center;

            Rgba &px = pixels[i][j];
            px.r = 0;
            px.g = 0;
            px.b = 0;
            px.a = 1;

            px_center = this->getPixelCenter(j, i, w, h);

            /* TODO:
             * rethink whether the view_ray should originate from camera
             * position or the pixel center.
             */
            Ray view_ray(this->eye, px_center.sub(this->eye).norm());

            RGB shade = this->shadeAlongRay(view_ray, surfaces, lights,
                                            MAX_RECURSIVE_LIMIT, -1);

            px.r = shade.r;
            px.g = shade.g;
            px.b = shade.b;

            progress.log((i + 1) * (j + 1), total_pixels);
        }
    }
    progress.done();
}


