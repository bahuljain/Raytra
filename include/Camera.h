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

static const int MAX_RECURSIVE_LIMIT = 20;

class Camera {
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

    Point getPixelCenter(int, int, float, float) const;

    RGB shadeAlongRay(const Ray &view_ray,
                      const vector<Surface *> &surfaces,
                      const vector<Light *> &lights,
                      int refl_limit,
                      int origin_surface_idx,
                      const BVHTree &surfacesTree,
                      int mode) const;

    void render(Array2D <Rgba> &,
                const vector<Surface *> &,
                const vector<Material *> &,
                const vector<Light *> &) const;
};


#endif //RAYTRA_CAMERA_H
