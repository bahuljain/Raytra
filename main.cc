#include <iostream>
#include "Vector.h"
#include "Point.h"
#include "Camera.h"


int main() {


    Camera cam = Camera(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);


    Point p1 = Point(1.0, 2.0, 3.0);
    Point p2 = Point(0.5, 1.9, 3.99);
    Vector v1 = - p1.sub(p2);

    std::cout << "The vector generated " << v1.i << "i + " << v1.j << "j + " << v1.k << "k" << std::endl;
    return 0;
}