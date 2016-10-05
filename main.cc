#include <iostream>
#include <vector>
#include <limits>
#include "include/Vector.h"
#include "include/Point.h"
#include "include/Camera.h"
#include "include/Ray.h"
#include "include/Sphere.h"


int getClosestSurface(const std::vector<Surface*> &vector, const Ray &ray);

int main(int ac, char** av) {
    // parse the scene file first

    // at this point we should have the camera object and other objects in the scene along with their material info.


    Camera cam = Camera(0, 0, 0, 1, 0, 0, 5, 10, 10, 10, 10);

    Sphere* sphere = new Sphere (8, 0, 0, 3);
//    Sphere sphere (8, 0, 0, 3);

    Material m (0.7, 0, 0, 0.5, 0.5, 0.5, 100.0, 0, 0, 0);

    sphere->setMaterial(m);

    std::vector<Surface*> surfaces;

    surfaces.push_back(sphere);

    float w = cam.right - cam.left;
    float h = cam.top - cam.bottom;


    for (int i = 0; i < cam.pw; i++) {
        for (int j = 0; j < cam.ph; j++) {
            Point px_center;
            px_center = cam.getPixelCenter(i, j, w, h);

            Ray ray (px_center, px_center.sub(cam.eye).norm());

            int closest_surface_idx = getClosestSurface(surfaces, ray);

            if (closest_surface_idx != -1) {
                Material mat = surfaces[closest_surface_idx]->getMaterial();
                /* mat.diffuse */
                // set the pixel rgb values to the diffuse colors of the material
            } else {
                // set pixel to black
            }

        }
    }

    return 0;
}

int getClosestSurface(const std::vector<Surface*> &surfaces, const Ray &ray) {
    float min_t = std::numeric_limits<float>::infinity();
    int min_i = -1;

    for (int i = 0; i < surfaces.size(); i++) {
        int t = surfaces[i]->getIntersection(ray);

        if (t >= 0 && t < min_t) {
            min_t = t;
            min_i = i;
        }
    }
    return min_i;
}