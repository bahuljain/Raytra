#include <cstdlib>
#include <cmath>
#include <fstream>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <limits>

#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include <ImfRgba.h>
#include <ImfRgbaFile.h>

#include "include/Vector.h"
#include "include/Point.h"
#include "include/Camera.h"
#include "include/Ray.h"

#include "include/Sphere.h"

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

// this is called from the parseSceneFile function, which uses
// it to get the float from the correspoding position on the line.
//
// return the corresponding token in the inString. Errors out
// if you've asked for more than are in the line.
//
// you really don't need to know what is going on in here, I think.
//
float getTokenAsFloat (string inString, int whichToken) {

    float thisFloatVal = 0.;    // the return value

    if (whichToken == 0) {
        cerr << "error: the first token on a line is a character!" << endl;
        exit (-1);
    }

    // c++ string class has no super-easy way to tokenize, let's use c's:
    char *cstr = new char [inString.size () + 1];

    strcpy (cstr, inString.c_str());

    char *p = strtok (cstr, " ");
    if (p == 0) {
        cerr << "error: the line has nothing on it!" << endl;
        exit (-1);
    }

    for (int i = 0; i < whichToken; i++) {
        p = strtok (0, " ");
        if (p == 0) {
            cerr << "error: the line is not long enough for your token request!" << endl;
            exit (-1);
        }
    }

    thisFloatVal = atof (p);

    delete[] cstr;

    return thisFloatVal;
}

//
// read the scene file.
//
// You'll need a few globals (or add arguments to this function): for the
// list of surfaces (geometric objects like spheres, triangles, planes) and
// another for the lights. These can be of type std::vector. You'll also
// need a global (or other parameter) for the camera.
//
// This should be pretty self-explanatory: it reads through the lines, and
// using the first character figures out what kind of command it is. It
// then calls the "getTokenAsFloat" routine to pull out the needed values.
// NOTE: since different commands take different number of arguments, you
// must take care not to call getTokenAsFloat for an index that is beyond the
// end of the line!
//
// One tricky bit: when a material is read in, we want
// to keep a pointer to it so that for the next geometric object read in ,
// we can add that material to the object. In the code that follows, I use the
// variable "lastSurfaceLoaded" to do that, but the code is commented out since
// I don't know the class names you will be using.
//
// Very minimal error check here. You might improve it slightly, but we'll
// only use "correct" scene files.
//
//
void parseSceneFile (char *filename, std::vector<Surface*> &surfaces, Camera* cam) {

    ifstream inFile(filename);    // open the file
    string line;

    if (! inFile.is_open ()) {  // if it's not open, error out.
        cerr << "can't open scene file" << endl;
        exit (-1);
    }

    Material lastMaterialLoaded = Material(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    // Note: you'll have to keep track of whatever the last material
    // you loaded in was, so you can apply it to any geometry that gets loaded.
    // So here, you'll have something like:
    //
    // myMaterialClass *lastMaterialLoaded = 0;  // 0 or maybe a default material?
    //
    // and each time you load in a new piece of geometry (sphere, triangle, plane)
    // you will set its material to lastMaterialLoaded.


    while (! inFile.eof ()) {   // go through every line in the file until finished

        getline (inFile, line); // get the line

        switch (line[0])  {     // we'll decide which command based on the first character

            //
            // geometry types:
            //
            // NOTE: whichever type of geo you load in, set its material to
            // be "lastMaterialLoaded"
            //
            case 's': {
                // it's a sphere, load in the parameters

                float x, y, z, r;
                x = getTokenAsFloat(line, 1);
                y = getTokenAsFloat(line, 2);
                z = getTokenAsFloat(line, 3);
                r = getTokenAsFloat(line, 4);

                Sphere *sphere = new Sphere(x, y, z, r);
                sphere->setMaterial(lastMaterialLoaded);

                surfaces.push_back(sphere);

                // build your sphere here from the parameters
                // i.e. you must call your sphere constructor and set its position
                // and radius from the above values. You must also put your new
                // sphere into the objects list (which can be global)
                // So something like;
                // mySphereClass *ms = new mySphereClass (x, y, z, r);   // make a new instance of your sphere class
                // ms->setMaterial (lastMaterialLoaded)
                // objectsList->push_back (ms);  // objectsList is a global std:vector<surface *> for example.

#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "got a sphere with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << r << endl;
#endif
                break;
            }
            case 't':   // triangle
                break;

            case 'p':   // plane
                break;

            case 'c': {  // camera
                // one trick here: the cameras pixel count (width, height) are integers,
                // so cast them.
                float x, y, z, vx, vy, vz, d, iw, ih;
                int pw, ph;

                x = getTokenAsFloat(line, 1);
                y = getTokenAsFloat(line, 2);
                z = getTokenAsFloat(line, 3);
                vx = getTokenAsFloat(line, 4);
                vy = getTokenAsFloat(line, 5);
                vz = getTokenAsFloat(line, 6);
                d = getTokenAsFloat(line, 7);
                iw = getTokenAsFloat(line, 8);
                ih = getTokenAsFloat(line, 9);
                pw = (int) getTokenAsFloat(line, 10);
                ph = (int) getTokenAsFloat(line, 11);

                cam->setValues (x, y, z, vx, vy, vz, d, iw, ih, pw, ph);

                break;
            }
            case 'l':   // light
                break;
                /*
                // slightly different from the rest, we need to examine the second param,
                // which is at the third position on the line:
                switch (line[2]) {
                    case 'p':   // point light
                        break;
                    case 'd':   // directional light
                        break;
                    case 'a':   // ambient light
                        break;

                }

                break;*/

            case 'm': {  // material
                // the trick here: we should keep a pointer to the last material we read in,
                // so we can apply it to any subsequent geometry. Say it's called "lastMaterialLoaded"
                // we migh then do something like this:
                //
                //  1. read in the 10 material parameters: dr, dg, db, sr, sg, sb, r, ir, ig, ib
                //  2. call lastMaterialLoaded->setMaterial(dr, dg, db,...);
                //
                float dr, dg, db, sr, sg, sb, r, ir, ig, ib;
                dr = getTokenAsFloat(line, 1);
                dg = getTokenAsFloat(line, 2);
                db = getTokenAsFloat(line, 3);
                sr = getTokenAsFloat(line, 4);
                sb = getTokenAsFloat(line, 5);
                sg = getTokenAsFloat(line, 6);
                r = getTokenAsFloat(line, 7);
                ir = getTokenAsFloat(line, 8);
                ig = getTokenAsFloat(line, 9);
                ib = getTokenAsFloat(line, 10);

                lastMaterialLoaded.setValues(dr, dg, db, sr, sg, sb, r, ir, ig, ib);

                break;
            }

            case '/':
                // don't do anything, it's a comment
                break;


                //
                // options
                //
            case 'o':   // make your own options if you wish
                break;
        }

    }
}

int getClosestSurface(const std::vector<Surface*> &surfaces, const Ray &ray) {
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
    // parse the scene file first
    if (argc != 2) {
        cerr << "usage: raytra scenefilename" << endl;
        return -1;
    }

    Camera* cam = new Camera(); // = new Camera(24.2, 29.3, 53.6, 0.0, 0.0, -1.0f, 35.0, 35.0, 25.0, 160, 120);
    std::vector<Surface*> surfaces;

    parseSceneFile (argv[1], surfaces, cam);

    // TODO: Need to add sanity checks for camera and surfaces parsed from scene

    float w = cam->right - cam->left;
    float h = cam->top - cam->bottom;

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

    return 0;
}