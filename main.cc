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

int getClosestSurface(const vector<Surface*> &surfaces, const Ray &ray) {
    float min_t = numeric_limits<float>::infinity();
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

int main(int argc, char** argv) {

    if (argc != 2) {
        cerr << "usage: raytra scenefilename" << endl;
        return -1;
    }

    Camera* cam = new Camera();
    vector<Surface*> surfaces;

    parseSceneFile (argv[1], surfaces, cam);

    // TODO: Need to add sanity checks for surfaces parsed from scene

    float w = cam->right - cam->left;
    float h = cam->top - cam->bottom;

    if (cam->pw == 0 && cam->ph == 0 && w == 0 && h == 0) {
        cout << "No camera and image plane in the scene" << endl;
        return -1;
    }

    Array2D<Rgba> pixels;
    pixels.resizeErase(cam->ph, cam->pw);

    for (int i = 0; i < cam->ph; i++) {
        for (int j = 0; j < cam->pw; j++) {

            Point px_center;
            px_center = cam->getPixelCenter(j, i, w, h);

            Ray ray (px_center, px_center.sub(cam->eye).norm());

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

    writeRgba("raytra_render.exr", &pixels[0][0], cam->pw, cam->ph);


    delete cam;

    for (unsigned int k = 0; k < surfaces.size(); k++) {
        delete surfaces[k];
    }

    return 0;
}