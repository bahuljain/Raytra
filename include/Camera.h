//
// Created by bahuljain on 10/3/16.
//

#ifndef RAYTRA_CAMERA_H
#define RAYTRA_CAMERA_H


#include "Point.h"
#include "Surface.h"
#include "Light.h"
#include "BVHTree.h"
#include <ImfRgba.h>
#include <ImfArray.h>

using namespace std;
using namespace Imf;

static const int RECURSIVE_LIMIT = 20;

class Camera {
private:
    Point getPixelSample(int i, int j,
                         float width, float height,
                         int p, int q, int strata) const;

    tuple<int, float> getClosestSurface(const BVHTree &surfaces,
                                        const Ray &ray, int mode) const;

    bool isIntercepted(const BVHTree &surfaces,
                       const Ray &ray, float t_max, int mode) const;

    RGB diffuseFromPointLights(const vector<PointLight *> &plights,
                               const BVHTree &surfaces,
                               const Surface *surface,
                               const Ray &view_ray,
                               const Point &intersection,
                               int mode) const;

    RGB diffuseFromSquareLights(const vector<SquareLight *> &slights,
                                const BVHTree &surfaces,
                                const Surface *surface,
                                const Ray &view_ray,
                                const Point &intersection,
                                int mode, int s_strata) const;

    RGB getShadeAlongRay(const Ray &view_ray,
                         const vector<PointLight *> &plights,
                         const vector<SquareLight *> &slights,
                         const AmbientLight &ambient,
                         const BVHTree &surfaces,
                         int refl_limit,
                         int origin_surface_idx,
                         int mode, int s_strata) const;

public:
    Point eye;
    Vector w;
    Vector u;
    Vector v;

    /* Focal length */
    float d;
    int pw, ph;

    float left, right, top, bottom;

    Camera();

    void setValues(float, float, float,
                   float, float, float,
                   float, float, float,
                   int, int);


    void render(Array2D <Rgba> &pixels,
                const vector<Surface *> &surfaces,
                const vector<Material *> &materials,
                const vector<PointLight *> &plights,
                const vector<SquareLight *> &slights,
                const AmbientLight &ambient,
                int mode, int p_strata, int s_strata) const;
};


#endif //RAYTRA_CAMERA_H
