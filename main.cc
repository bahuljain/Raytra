#include <iostream>
#include <vector>
#include <limits>
#include "include/Vector.h"
#include "include/Point.h"
#include "include/Camera.h"
#include "include/Ray.h"
#include "include/Sphere.h"

#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include <ImfRgba.h>
#include <ImfRgbaFile.h>

using namespace Imf;
using namespace std;
using namespace Imath;

int getClosestSurface(const std::vector<Surface*> &vector, const Ray &ray);
void writeRgba (const char fileName[], const Rgba *pixels, int width, int height);

int main(int ac, char** av) {
    // parse the scene file first

    // at this point we should have the camera object and other objects in the scene along with their material info.

    Camera cam = Camera(24.2, 29.3, 53.6, 0.0, 0.0, -1.0f, 35.0, 35.0, 25.0, 160, 120);

    Sphere* sphere1 = new Sphere (0.0, 13.67, -62.57f, 10.0);

    Material m (0.7, 0, 0, 0.5, 0.5, 0.5, 100.0, 0, 0, 0);
    sphere1->setMaterial(m);

    std::vector<Surface*> surfaces;
    surfaces.push_back(sphere1);

    float w = cam.right - cam.left;
    float h = cam.top - cam.bottom;

    Array2D<Rgba> pixels;
    pixels.resizeErase(cam.ph, cam.pw);

    for (int i = 0; i < cam.ph; i++) {
        for (int j = 0; j < cam.pw; j++) {
            Point px_center;
            px_center = cam.getPixelCenter(j, i, w, h);

            Ray ray (px_center, px_center.sub(cam.eye).norm());

            int closest_surface_idx = getClosestSurface(surfaces, ray);

            Rgba &px = pixels[i][j];

            if (closest_surface_idx != -1) {
                Material mat = surfaces[closest_surface_idx]->getMaterial();

                px.r = mat.diffuse.r;
                px.g = mat.diffuse.g;
                px.b = mat.diffuse.b;
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

    writeRgba("raytra_render.exr", &pixels[0][0], cam.pw, cam.ph);

    return 0;
}

int getClosestSurface(const std::vector<Surface*> &surfaces, const Ray &ray) {
    float min_t = std::numeric_limits<float>::infinity();
    int min_i = -1;

    for (unsigned int i = 0; i < surfaces.size(); i++) {
        int t = surfaces[i]->getIntersection(ray);

        if (t >= 0 && t < min_t) {
            min_t = t;
            min_i = i;
        }
    }
    return min_i;
}

void writeRgba (const char fileName[], const Rgba *pixels,
           int width, int height)
{
    //
    // Write an RGBA image using class RgbaOutputFile.
    //
    //	- open the file
    //	- describe the memory layout of the pixels
    //	- store the pixels in the file
    //


    RgbaOutputFile file (fileName, width, height, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}