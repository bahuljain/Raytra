//
// Created by bahuljain on 10/3/16.
//

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

tuple<int, float> Camera::getClosestSurface(
        const vector<Surface *> &surfaces,
        const Ray &ray) const {

    float min_t = numeric_limits<float>::infinity();
    int min_i = -1;

    for (unsigned int i = 0; i < surfaces.size(); i++) {
        float t = surfaces[i]->getIntersection(ray);

        if (t >= 0 && t < min_t) {
            min_t = t;
            min_i = i;
        }
    }
    return make_tuple(min_i, min_t);
}

RGB Camera::phongShading(const Surface *surface,
                 const Light *light,
                 const Ray &light_ray,
                 const Ray &view_ray,
                 const Point &intersection) const {
    float r, g, b, d2, diffuse_factor, specular_factor;
    Vector v, normal, I, bisector;
    Material *material;

    /* Vector to the viewer */
    v = -view_ray.direction;

    if (!surface->isFrontFaced(view_ray)) {
        return RGB(1, 1, 0);
    }

    material = surface->getMaterial();

    /* Distance of light from the point of intersection */
    d2 = light->position.distance2(intersection);

    /* Accounting for zero distance, in which case there shouldn't be any distance attenuation */
    if (d2 == 0)
        d2 = 1;

    /* Vector normal to the surface at the given intersection point */
    normal = surface->getSurfaceNormal(intersection);

    /* Vector to the light source */
    I = -light_ray.direction;

    /* Vector bisecting the view vector and light vector */
    bisector = v.plus(I).norm();

    diffuse_factor = light->intensity * fmaxf(0, normal.dot(I));
    specular_factor = light->intensity * powf(fmaxf(0, normal.dot(bisector)), material->phong);

    r = (material->diffuse.r * diffuse_factor
         + material->specular.r * specular_factor) * light->color.r / d2;

    g = (material->diffuse.g * diffuse_factor
         + material->specular.g * specular_factor) * light->color.g / d2;

    b = (material->diffuse.b * diffuse_factor
         + material->specular.b * specular_factor) * light->color.b / d2;

    return RGB(r, g, b);
}

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
            px_center = this->getPixelCenter(j, i, w, h);

            /* TODO:
             * rethink whether the view_ray should originate from camera position or the
             * pixel center.
             */
            Ray view_ray(this->eye, px_center.sub(this->eye).norm());

            /* Get closest surface along the ray */
            std::tuple<int, float> closest_surface = this->getClosestSurface(surfaces, view_ray);

            int closest_surface_idx = std::get<0>(closest_surface);
            float t = std::get<1>(closest_surface);

            Rgba &px = pixels[i][j];
            px.r = 0;
            px.g = 0;
            px.b = 0;
            px.a = 1;

            if (closest_surface_idx != -1) {
                Point intersection = view_ray.getPointOnIt(t);

                for (Light *light : lights) {
                    Ray light_ray(light->position, intersection.sub(light->position).norm());
                    float t_max = light_ray.getOffsetFromOrigin(intersection);
                    bool isIntercepted = false;

                    for (Surface *surface : surfaces) {
                        if (surface->intercepts(light_ray, t_max)) {
                            isIntercepted = true;
                            break;
                        }
                    }

                    if (!isIntercepted) {
                        Surface *surface = surfaces[closest_surface_idx];

                        RGB shade = this->phongShading(surface, light, light_ray, view_ray, intersection);

                        px.r += shade.r;
                        px.g += shade.g;
                        px.b += shade.b;
                    }
                }
            }
            progress.log((i + 1) * (j + 1), total_pixels);
        }
    }
    progress.done();
}