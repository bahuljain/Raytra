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
                 vector<PointLight *> &plights,
                 vector<SquareLight *> &slights) {
    delete cam;
    for (auto *surface : surfaces) delete surface;
    for (auto *material : materials) delete material;
    for (auto *light : plights) delete light;
    for (auto *light : slights) delete light;
}

int main(int argc, char **argv) {

    if (argc < 5) {
        cerr << "usage: raytra scenefilename outputfilename.exr "
                "<primary_samples> <shadow_samples>" << endl;
        return -1;
    }

    Camera *cam = new Camera();
    vector<Surface *> surfaces;
    vector<Material *> materials;
    vector<PointLight *> plights;
    vector<SquareLight *> slights;
    AmbientLight ambient;

    parseSceneFile(argv[1], surfaces, materials, plights, slights,
                   ambient, cam);

    cout << "Surfaces: " << surfaces.size() << endl;
    cout << "Materials: " << materials.size() - 1 << endl;
    cout << "Lights: " << slights.size() + plights.size() + 1 << endl << endl;

    Array2D <Rgba> pixels;

    int primary_samples = atoi(argv[3]);
    int shadow_samples = atoi(argv[4]);

    if (primary_samples < 1 || primary_samples > 100) {
        cerr << "error: too less/many number of primary samples" << endl;
        return -1;
    }

    if (shadow_samples < 1 || shadow_samples > 100) {
        cerr << "error: too less/many number of shadow samples" << endl;
        return -1;
    }

    // TODO: add mode info -- for now defaulting to mode = -1
    cam->render(pixels, surfaces, materials, plights, slights, ambient, -1,
                primary_samples, shadow_samples);

    writeRgba(argv[2], &pixels[0][0], cam->pw, cam->ph);

    cleanMemory(cam, surfaces, materials, plights, slights);
    return 0;
}
