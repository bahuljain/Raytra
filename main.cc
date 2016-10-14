#include <fstream>
#include <vector>

#include <ImfStringAttribute.h>
#include <ImfArray.h>
#include <ImfRgba.h>
#include <ImfRgbaFile.h>
#include <limits>

#include "include/parser.h"

using namespace Imf;
using namespace std;
using namespace Imath;


void writeRgba(const char fileName[], const Rgba *pixels,
               int width, int height) {
    //
    // Write an RGBA image using class RgbaOutputFile.
    //
    //	- open the file
    //	- describe the memory layout of the pixels
    //	- store the pixels in the file
    //

    RgbaOutputFile file(fileName, width, height, WRITE_RGBA);
    file.setFrameBuffer(pixels, 1, width);
    file.writePixels(height);
}

std::tuple<int, float> getClosestSurface(const vector<Surface *> &surfaces, const Ray &ray) {
    float min_t = numeric_limits<float>::infinity();
    int min_i = -1;

    for (unsigned int i = 0; i < surfaces.size(); i++) {
        float t = surfaces[i]->getIntersection(ray);

        if (t >= 0 && t < min_t) {
            min_t = t;
            min_i = i;
        }
    }
    return std::make_tuple(min_i, min_t);
}

RGB phongShading(const Surface *surface,
                 const Light *light,
                 const Material *material,
                 const Ray &ray,
                 const Point &intersection) {
    float r, g, b, d2, diffuse_factor, specular_factor;
    Vector v, normal, I, bisector;

    /* Distance of light from the point of intersection */
    d2 = light->position.distance2(intersection);

    /* Vector to the viewer */
    v = -ray.direction;

    /* Vector normal to the surface at the given intersection point */
    normal = surface->getSurfaceNormal(intersection);

    /* Vector to the light source */
    I = light->position.sub(intersection).norm();

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

void cleanMemory(Camera *cam,
                 Light *light,
                 std::vector<Surface *> &surfaces,
                 std::vector<Material *> &materials,
                 std::vector<Light *> &lights) {
    delete cam;
    delete light;

    for (Surface *surface : surfaces) {
        delete surface;
    }

    for (Material *material : materials) {
        delete material;
    }

    for (Light *light : lights) {
        delete light;
    }
}

void render(Array2D <Rgba> &pixels,
            const Camera *cam,
            const Light *light,
            const vector<Surface *> &surfaces,
            const vector<Material *> &materials,
            const vector<Light *> &lights) {

    float w = cam->right - cam->left;
    float h = cam->top - cam->bottom;

    pixels.resizeErase(cam->ph, cam->pw);

    for (int i = 0; i < cam->ph; i++) {
        for (int j = 0; j < cam->pw; j++) {
            Point px_center;
            px_center = cam->getPixelCenter(j, i, w, h);

            Ray ray(px_center, px_center.sub(cam->eye).norm());

            /* Get closest surface along the ray */
            std::tuple<int, float> closest_surface = getClosestSurface(surfaces, ray);
            int closest_surface_idx = std::get<0>(closest_surface);
            float t = std::get<1>(closest_surface);

            Rgba &px = pixels[i][j];

            if (closest_surface_idx != -1) {
                Surface *surface = surfaces[closest_surface_idx];
                Point intersection = ray.getPointOnIt(t);
                Material *material = surface->getMaterial();
                RGB shade = phongShading(surface, light, material, ray, intersection);

                px.r = shade.r;
                px.g = shade.g;
                px.b = shade.b;
                px.a = 1;

            } else {
                // set pixel to black
                px.r = 0;
                px.g = 0;
                px.b = 0;
                px.a = 1;
            }

        }
    }
}

int main(int argc, char **argv) {

    if (argc < 2) {
        cerr << "usage: raytra scenefilename outputfilename.exr" << endl;
        return -1;
    }

    Camera *cam = new Camera();
    Light *light = new Light();
    vector<Surface *> surfaces;
    vector<Material *> materials;
    vector<Light *> lights;

    parseSceneFile(argv[1], surfaces, materials, cam, light);

    Array2D <Rgba> pixels;

    render(pixels, cam, light, surfaces, materials, lights);

    if (argc == 2) {
        writeRgba("hw1.2.exr", &pixels[0][0], cam->pw, cam->ph);
    } else {
        writeRgba(argv[2], &pixels[0][0], cam->pw, cam->ph);
    }

    cleanMemory(cam, light, surfaces, materials, lights);

    return 0;
}