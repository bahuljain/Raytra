#include <fstream>
#include <vector>
#include <ImfArray.h>
#include <ImfRgba.h>
#include <ImfRgbaFile.h>
#include "include/Parser.h"

using namespace Imf;
using namespace std;

/// Write an RGBA image using class RgbaOutputFile.
///
/// - open the file
///	- describe the memory layout of the pixels
///	- store the pixels in the file
/// @param fileName - name of the output image file.
/// @param pixels - a two dimensional array holding the image pixels.
/// @param width - number of columns (width) in the image.
/// @param height - number of rows (height) in the image.
void writeRgba(const char fileName[], const Rgba *pixels,
               int width, int height) {

    RgbaOutputFile file(fileName, width, height, WRITE_RGBA);
    file.setFrameBuffer(pixels, 1, width);
    file.writePixels(height);
}

void cleanMemory(Camera *cam,
                 vector<Surface *> &surfaces,
                 vector<Material *> &materials,
                 vector<Light *> &lights) {
    delete cam;

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

int main(int argc, char **argv) {

    if (argc < 3) {
        cerr << "usage: raytra scenefilename outputfilename.exr <mode>" << endl;
        return -1;
    }

    Camera *cam = new Camera();
    vector<Surface *> surfaces;
    vector<Material *> materials;
    vector<Light *> lights;

    parseSceneFile(argv[1], surfaces, materials, lights, cam);
    cout << "Surfaces: " << surfaces.size() << endl;
    cout << "Materials: " << materials.size() - 1 << endl;
    cout << "Lights: " << lights.size() << endl << endl;

    Array2D <Rgba> pixels;

    int strata = 4;

    if (argc == 3)
        cam->render(pixels, surfaces, materials, lights, -1, strata);
    else {
        int mode = atoi(argv[3]);

        if (mode != 0 && mode != 1) {
            cerr << "error: incorrect mode of operation" << endl;
            return -1;
        }

        cam->render(pixels, surfaces, materials, lights, mode, strata);
    }

    writeRgba(argv[2], &pixels[0][0], cam->pw, cam->ph);

    cleanMemory(cam, surfaces, materials, lights);
    return 0;
}



