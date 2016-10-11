//
// Created by bahuljain on 10/6/16.
//

#include <cstdlib>
#include <fstream>
#include <cstring>
#include "include/parser.h"
#include "include/Sphere.h"
#include "include/Plane.h"

// this is called from the parseSceneFile function, which uses
// it to get the float from the correspoding position on the line.
//
// return the corresponding token in the inString. Errors out
// if you've asked for more than are in the line.
//
// you really don't need to know what is going on in here, I think.
//
float getTokenAsFloat (string inString, int whichToken) {

    float thisFloatVal = 0.;  // the return value

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
void parseSceneFile (char *filename, std::vector<Surface*> &surfaces,
                     Camera* cam, Light* light, AmbientLight* ambient) {
    int Cams = 0;

    ifstream inFile(filename);    // open the file
    string line;

    if (! inFile.is_open ()) {  // if it's not open, error out.
        cerr << "can't open scene file" << endl;
        exit (-1);
    }

    /* Most recently added material */
    Material lastMaterialLoaded (0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

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

#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "got a sphere with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << r << endl;
#endif
                break;
            }
            case 't':   // triangle
                break;

            case 'p': {  // plane
                float nx, ny, nz, d;

                nx = getTokenAsFloat(line, 1);
                ny = getTokenAsFloat(line, 2);
                nz = getTokenAsFloat(line, 3);
                d = getTokenAsFloat(line, 4);

                Plane *plane = new Plane(nx, ny, nz, d);
                plane->setMaterial(lastMaterialLoaded);

                surfaces.push_back(plane);
            }
                break;

            case 'c': {  // camera
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

                cam->setValues(x, y, z, vx, vy, vz, d, iw, ih, pw, ph);

                Cams++;

                break;
            }
            case 'l':   // light
                // slightly different from the rest, we need to examine the second param,
                // which is at the third position on the line:
                switch (line[2]) {
                    case 'p': { // point light
                        float x, y, z, r, g, b;

                        x = getTokenAsFloat(line, 2);
                        y = getTokenAsFloat(line, 3);
                        z = getTokenAsFloat(line, 4);
                        r = getTokenAsFloat(line, 5);
                        g = getTokenAsFloat(line, 6);
                        b = getTokenAsFloat(line, 7);

                        light->setValues(x, y, z, r, g, b, 1.0);

                        break;
                    }
/*
                    case 'd':   // directional light
                        break;
*/
                    case 'a':   // ambient light
                        float r, g, b;

                        r = getTokenAsFloat(line, 2);
                        g = getTokenAsFloat(line, 2);
                        b = getTokenAsFloat(line, 2);

                        ambient->setValues(r, g, b);
                        break;

                }

                break;

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

        if (Cams != 1) {
            cerr << "error: scene should have exactly 1 camera!" << endl;
            exit(-1);
        }

    }
}