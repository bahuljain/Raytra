/**
 * @file    Camera.cc
 * @author  Bahul Jain
 * @date    10/3/16
 * @brief   Holds all constructors and members of the Camera class.
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
Point Camera::getPixelSample(int i, int j, float width, float height,
                             int p, int q, int strata) const {
    Point sample;

    float x_d = (p + ((float) rand() / RAND_MAX)) / strata;
    float y_d = (q + ((float) rand() / RAND_MAX)) / strata;

    float x = left + width * (i + x_d) / pw;
    float y = bottom + height * (j + y_d) / ph;

    sample = eye
            .moveAlong(u.times(x))
            .moveAlong(v.times(y))
            .moveAlong(w.times(-d));

//    sample.printPoint();

    return sample;
}

/**
 * @name    getClosestSurface
 * @brief   Finds the closest surface to the camera along a given ray.
 *
 * @param surfacesTree   -
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
tuple<int, float> Camera::getClosestSurface(const BVHTree &surfacesTree,
                                            const Ray &ray, int mode) const {
    if (mode != 0)
        return surfacesTree.getClosestSurface(ray, mode);

    float min_t = numeric_limits<float>::infinity();
    int min_i = -1;

    for (int i = 0; i < surfacesTree.getTotalSurfaces(); i++) {
        float t = surfacesTree.getSurfaceAt(i)->getIntersection(ray);

        if (t >= 0.01 && t < min_t) {
            min_t = t;
            min_i = i;
        }
    }
    return make_tuple(min_i, min_t);
}

/**
 * @name    isIntercepted
 * @brief   Determines if a surface intercepts the ray before it reaches it
 *          final destination.
 *
 * @param surfacesTree -
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
bool Camera::isIntercepted(const BVHTree &surfacesTree,
                           const Ray &ray, float t_max, int mode) const {
    if (mode != 0)
        return surfacesTree.isIntercepted(ray, t_max, mode);

    for (int i = 0; i < surfacesTree.getTotalSurfaces(); i++) {
        float t = surfacesTree.getSurfaceAt(i)->getIntersection(ray);

        if (t >= 0 && t < fabsf(t_max - 0.01f)) return true;
    }
    return false;
}

/**
 * @name diffuseFromPointLights
 * @brief obtains the shade on the surface from all the point lights in the
 * scene.
 *
 *
 * */
RGB Camera::diffuseFromPointLights(const vector<PointLight *> &plights,
                                   const BVHTree &surfacesTree,
                                   const Surface *surface,
                                   const Ray &view_ray,
                                   const Point &intersection,
                                   int mode) const {
    RGB shade(0, 0, 0);
    for (PointLight *light : plights) {
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
        if (!isIntercepted(surfacesTree, light_ray, t_max, mode))
            shade.add(
                    surface->phongShading(light->color, light_ray, view_ray,
                                          intersection, mode));
    }
    return shade;
}

/**
 * @name diffuseFromSquareLight
 * @brief obtains the diffuse shading contributed by all the square lights in
 * the scene.
 *
 * @param slights  - a list of all the sqaure lights in the scene
 * @param s_strata - the number of samples that need to be collected from the
 *                   area light are determined by this value.
 *
 * @see diffuseFromPointLights
 * @details For the most part the process of shading is exactly the same as
 * that for point lights. But here since its an area light, we need to collect
 * multiple samples from different sections of the area light. For each sample
 * the shade obtained will be attenuated by the angle the light ray makes
 * with the normal direction of the square light. Lastly each sample is
 * collected, added together and normalized.
 */
RGB Camera::diffuseFromSquareLights(const vector<SquareLight *> &slights,
                                    const BVHTree &surfacesTree,
                                    const Surface *surface,
                                    const Ray &view_ray,
                                    const Point &intersection,
                                    int mode, int s_strata) const {
    RGB shade(0, 0, 0);
    for (SquareLight *light :slights) {
        for (int p = 0; p < s_strata; p++) {
            for (int q = 0; q < s_strata; q++) {
                /* Obtaining a random sample point on the area light */
                Point light_sample = light->getLightSample(p, q, s_strata);

                Ray light_ray(light_sample,
                              intersection.sub(light_sample).norm());

                float t_max = light_ray.getOffsetFromOrigin(intersection);

                if (!isIntercepted(surfacesTree, light_ray,
                                   t_max, mode)) {

                    /*
                     * Attenuating the shade of the light according to the
                     * angle made by the light ray and normal of the light
                     */
                    float cos = fmaxf(0, light_ray.direction.dot(light->w));
                    RGB light_color = light->color.times(cos);

                    RGB c = surface->phongShading(light_color, light_ray,
                                                  view_ray, intersection,
                                                  mode);

                    /* Normalizing the shade obtained and accumulating it. */
                    float avg_factor = 1.0f / (s_strata * s_strata);
                    shade.add(c.times(avg_factor));
                }
            }
        }
    }
    return shade;
}

/**
 * @name    getShadeAlongRay
 * @brief   Computes the shading along the given view ray
 *
 * @param view_ray    - the ray along which shading needs to be computed.
 * @param lights      - a list of all the light sources in the scene.
 * @param refl_limit  - the number of reflections allowed before the light
 *                      fades away.
 * @param origin_surface_idx - the index of the surface from which the ray is
 *                      coming from. Set to any negative value if ray is coming
 *                      from the viewer and not some surface.
 * @returns           - the RGB value (spectral distribution) obtained along
 *                      the given view ray
 */
RGB Camera::getShadeAlongRay(const Ray &view_ray,
                             const vector<PointLight *> &plights,
                             const vector<SquareLight *> &slights,
                             const AmbientLight &ambient,
                             const BVHTree &surfacesTree,
                             int refl_limit,
                             int origin_surface_idx,
                             int mode, int s_strata) const {
    RGB shade(0, 0, 0);

    /* No reflections beyond a limit */
    if (refl_limit == 0) return shade;

    /* Get closest surface along the ray */
    tuple<int, float> closest_surface;
    closest_surface = getClosestSurface(surfacesTree, view_ray, mode);

    int closest_surface_idx = get<0>(closest_surface);
    float t = get<1>(closest_surface);

    if (closest_surface_idx != -1) {
        Point intersection = view_ray.getPointOnIt(t);
        Surface *surface = surfacesTree.getSurfaceAt(closest_surface_idx);

        /* Get diffuse shading from all Point & Square Lights */
        shade.add(diffuseFromPointLights(plights, surfacesTree, surface,
                                         view_ray, intersection, mode));
        shade.add(diffuseFromSquareLights(slights, surfacesTree, surface,
                                          view_ray, intersection,
                                          mode, s_strata));

        /*
         * Ambient Light Shading
         * Note: Only add ambient light if computing shading for view ray
         * and not reflected/refracted rays.
         */
        if (origin_surface_idx == -1)
            shade.add(surface->getDiffuseComponent().scaleRGB(ambient.color));

        /*
         * If the surface is reflective and is front-faced with respect to
         * the view ray then compute shading from the reflected ray.
         */
        bool isFrontFaced = mode == 1 || surface->isFrontFacedTo(view_ray);
        if (surface->isReflective() && isFrontFaced) {
            Vector normal = (mode == 1)
                            ? surface->bbox->getSurfaceNormal(intersection)
                            : surface->getSurfaceNormal(intersection);

            /*
             * Directional vector generated when the view_ray reflects off the
             * surface.
             */
            Vector reflected_vector = view_ray.direction
                    .plus(-normal.times(2 * view_ray.direction.dot(normal)))
                    .norm();

            Ray reflected_ray(intersection, reflected_vector);

            /* The shade obtained from the reflected ray */
            RGB reflection = this->getShadeAlongRay(reflected_ray, plights,
                                                    slights, ambient,
                                                    surfacesTree,
                                                    refl_limit - 1,
                                                    closest_surface_idx, mode,
                                                    s_strata);

            shade.add(reflection.scaleRGB(surface->getReflectiveComponent()));
        }
    }
    return shade;
}

/**
 * @name    render
 * @brief   Renders the image using the ray tracing algorithm.
 *
 * @param pixels    - a two dimensional array of pixels representing the image.
 * @param materials - a vector of all the materials used.
 * @param plights   - a vector of all the point lights in the scene.
 * @param slights   - a vector of all the square lights in the scene.
 * @param ambient   - an ambient light added to the scene.
 * @param mode      - @see README.md - Run Modes
 * @param p_strata  - number of primary ray samples
 * @param s_strata  - number of area light (shadow ray) samples
 *
 * @details For every pixel in the image, construct a ray that originates
 *          from the camera eye and passes via the pixel center. Now trace
 *          the ray in the scene to find intersection with objects. The
 *          closest object encountered along the ray will get rendered at
 *          that pixel. Use the various light sources and materials to obtain
 *          the shading for each pixel.
 */
void Camera::render(Array2D <Rgba> &pixels, const vector<Surface *> &surfaces,
                    const vector<Material *> &materials,
                    const vector<PointLight *> &plights,
                    const vector<SquareLight *> &slights,
                    const AmbientLight &ambient,
                    int mode, int p_strata, int s_strata) const {

    float w = this->right - this->left;
    float h = this->top - this->bottom;
    float total_pixels = this->ph * this->pw;

    pixels.resizeErase(this->ph, this->pw);


    BVHTree surfaceTree(&surfaces);

    if (mode == 0) {
        cout << "Rendering without acceleration" << endl;
        goto render;
    }

    if (mode == -1)
        cout << "Rendering with acceleration" << endl;
    if (mode == 1)
        cout << "Rendering only bounding boxes" << endl;

    surfaceTree.makeBVHTree();

    render:
    cout << endl;

    ProgressBar progress = ProgressBar();
    progress.start();

    for (int i = 0; i < this->ph; i++) {
        for (int j = 0; j < this->pw; j++) {

            Rgba &px = pixels[i][j];
            px.r = px.g = px.b = 0;
            px.a = 1;

            for (int p = 0; p < p_strata; p++) {
                for (int q = 0; q < p_strata; q++) {
                    Point px_sample;

                    px_sample = this->getPixelSample(j, i, w, h, p, q,
                                                     p_strata);

                    // TODO: should this ray originate from px_sample or eye?
                    Ray view_ray(this->eye, px_sample.sub(this->eye).norm());

                    RGB shade = getShadeAlongRay(view_ray, plights, slights,
                                                 ambient, surfaceTree,
                                                 RECURSIVE_LIMIT, -1,
                                                 mode, s_strata);

                    px.r += shade.r;
                    px.g += shade.g;
                    px.b += shade.b;
                }
            }

            int total_samples = p_strata * p_strata;

            px.r /= total_samples;
            px.g /= total_samples;
            px.b /= total_samples;

            progress.log((i + 1) * (j + 1), total_pixels);
        }
    }
    progress.done();
}
