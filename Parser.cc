//
// Created by bahuljain on 10/6/16.
//

#include <cstdlib>
#include <fstream>
#include <cstring>
#include <sstream>
#include "include/Parser.h"
#include "include/Sphere.h"
#include "include/Plane.h"
#include "include/Triangle.h"
#include "include/PointLight.h"

// this is called from the parseSceneFile function, which uses
// it to get the float from the correspoding position on the line.
//
// return the corresponding token in the inString. Errors out
// if you've asked for more than are in the line.
//
// you really don't need to know what is going on in here, I think.
//
float getTokenAsFloat(string inString, int whichToken) {

    float thisFloatVal = 0.;  // the return value

    if (whichToken == 0) {
        cerr << "error: the first token on a line is a character!" << endl;
        exit(-1);
    }

    // c++ string class has no super-easy way to tokenize, let's use c's:
    char *cstr = new char[inString.size() + 1];

    strcpy(cstr, inString.c_str());

    char *p = strtok(cstr, " ");
    if (p == 0) {
        cerr << "error: the line has nothing on it!" << endl;
        exit(-1);
    }

    for (int i = 0; i < whichToken; i++) {
        p = strtok(0, " ");

        if (p == 0) {
            cerr << "error: the line is not long enough for your token request!"
                 << endl;
            exit(-1);
        }
    }

    thisFloatVal = atof(p);

    delete[] cstr;

    return thisFloatVal;
}

// Given the name of a wavefront (OBJ) file that consists JUST of
// vertices, triangles, and comments, read it into the tris and verts
// vectors.
//
// tris is a vector of ints that is 3*n long, where n is the number of
// triangles. The ith triangle has vertex indexes 3*i, 3*i+1, and 3*i+2.
//
// The ith triangle has vertices:
//
//     verts[3*tris[3*i]], verts[3*tris[3*i]+1], verts[3*tris[3*i]+2],
//     verts[3*tris[3*i+1]], verts[3*tris[3*i+1]+1], verts[3*tris[3*i+1]+2],
//     verts[3*tris[3*i+2]], verts[3*tris[3*i+2]+1], verts[3*tris[3*i+2]+2]
//
// given in counterclockwise order with respect to the surface normal
//
// If you are using the supplied Parser class, you should probably make this
// a method on it: Parser::read_wavefront_file().
//
void read_wavefront_file(
        const char *file,
        vector<int> &tris,
        vector<float> &verts) {

    // clear out the tris and verts vectors:
    tris.clear();
    verts.clear();

    ifstream in(file);
    char buffer[1025];
    string cmd;

    for (int line = 1; in.good(); line++) {
        in.getline(buffer, 1024);
        buffer[in.gcount()] = 0;

        cmd = "";

        istringstream iss(buffer);

        iss >> cmd;

        if (cmd[0] == '#' or cmd.empty()) {
            // ignore comments or blank lines
            continue;
        } else if (cmd == "v") {
            // got a vertex:

            // read in the parameters:
            double pa, pb, pc;
            iss >> pa >> pb >> pc;

            verts.push_back(pa);
            verts.push_back(pb);
            verts.push_back(pc);
        } else if (cmd == "f") {
            // got a face (triangle)

            // read in the parameters:
            int i, j, k;
            iss >> i >> j >> k;

            // vertex numbers in OBJ files start with 1, but in C++ array
            // indices start with 0, so we're shifting everything down by
            // 1
            tris.push_back(i - 1);
            tris.push_back(j - 1);
            tris.push_back(k - 1);
        } else {
            std::cerr << "Parser error: invalid command at line " << line
                      << std::endl;
        }

    }

    in.close();

    //   std::cout << "found this many tris, verts: " << tris.size () / 3.0 << "  " << verts.size () / 3.0 << std::endl;
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
void parseSceneFile(char *filename,
                    vector<Surface *> &surfaces,
                    vector<Material *> &materials,
                    vector<Light *> &lights,
                    Camera *cam) {
    int Cams = 0;

    ifstream inFile(filename);    // open the file
    string line;

    if (!inFile.is_open()) {  // if it's not open, error out.
        cerr << "can't open scene file" << endl;
        exit(-1);
    }

    /* Most recently added material and a default in the beginning */
    Material *lastMaterial = new Material();
    materials.push_back(lastMaterial);

    while (!inFile.eof()) {   // go through every line in the file until finished

        getline(inFile, line); // get the line

        switch (line[0]) {     // we'll decide which command based on the first character

            case '/':
                // don't do anything, it's a comment
                break;

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
                sphere->setMaterial(lastMaterial);

                surfaces.push_back(sphere);

#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "got a sphere with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << r << endl;
#endif
                break;
            }
            case 't': {  // triangle
                float x1, y1, z1, x2, y2, z2, x3, y3, z3;

                x1 = getTokenAsFloat(line, 1);
                y1 = getTokenAsFloat(line, 2);
                z1 = getTokenAsFloat(line, 3);
                x2 = getTokenAsFloat(line, 4);
                y2 = getTokenAsFloat(line, 5);
                z2 = getTokenAsFloat(line, 6);
                x3 = getTokenAsFloat(line, 7);
                y3 = getTokenAsFloat(line, 8);
                z3 = getTokenAsFloat(line, 9);

                Triangle *triangle = new Triangle(x1, y1, z1, x2, y2, z2, x3,
                                                  y3, z3);
                triangle->setMaterial(lastMaterial);

                surfaces.push_back(triangle);

                break;
            }
            case 'p': {  // plane
                /*
                float nx, ny, nz, d;

                nx = getTokenAsFloat(line, 1);
                ny = getTokenAsFloat(line, 2);
                nz = getTokenAsFloat(line, 3);
                d = getTokenAsFloat(line, 4);

                Plane *plane = new Plane(nx, ny, nz, d);
                plane->setMaterial(lastMaterial);

                surfaces.push_back(plane);
                */
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

                        PointLight *light = new PointLight(x, y, z,
                                                           r, g, b, 1.0);
                        lights.push_back(light);

                        break;
                    }
/*
                    case 'd':   // directional light
                        break;
*/
                    case 'a': {  // ambient light
                        float r, g, b;

                        r = getTokenAsFloat(line, 2);
                        g = getTokenAsFloat(line, 2);
                        b = getTokenAsFloat(line, 2);

                        AmbientLight *light = new AmbientLight(r, g, b);
                        lights.push_back(light);

                        break;
                    }
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

                lastMaterial = new Material(dr, dg, db, sr, sg, sb, r, ir, ig,
                                            ib);
                materials.push_back(lastMaterial);

                break;
            }

            case 'w': { // read .obj file
                string filename;
                vector<int> triangles;
                vector<float> vertices;

                filename = line.substr(2);

                read_wavefront_file(filename.c_str(), triangles, vertices);

                for (unsigned int i = 0; i < triangles.size() / 3; i++) {
                    Triangle *triangle;
                    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

                    x1 = vertices[3 * triangles[3 * i]];
                    y1 = vertices[3 * triangles[3 * i] + 1];
                    z1 = vertices[3 * triangles[3 * i] + 2];

                    x2 = vertices[3 * triangles[3 * i + 1]];
                    y2 = vertices[3 * triangles[3 * i + 1] + 1];
                    z2 = vertices[3 * triangles[3 * i + 1] + 2];

                    x3 = vertices[3 * triangles[3 * i + 2]];
                    y3 = vertices[3 * triangles[3 * i + 2] + 1];
                    z3 = vertices[3 * triangles[3 * i + 2] + 2];

                    triangle = new Triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);
                    triangle->setMaterial(lastMaterial);
                    surfaces.push_back(triangle);
                }

                break;
            }

                //
                // options
                //
            case 'o':   // make your own options if you wish
                break;
        }
    }

    if (Cams != 1) {
        cerr << "error: scene should have exactly 1 camera!" << endl;
        exit(-1);
    }
}