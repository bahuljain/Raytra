//
// Created by bahuljain on 10/6/16.
//

#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Surface.h"
#include "AmbientLight.h"
#include <iostream>

using namespace std;

#ifndef RAYTRA_PARSER_H
#define RAYTRA_PARSER_H

#endif //RAYTRA_PARSER_H


void parseSceneFile(char *,
                    vector<Surface *> &,
                    vector<Material *> &,
                    vector<Light *> &,
                    Camera *);

float getTokenAsFloat(string, int);