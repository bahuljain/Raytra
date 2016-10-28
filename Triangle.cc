//
// Created by bahuljain on 10/6/16.
//

#include "include/Triangle.h"

Triangle::Triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3) {
    this->a = Point(x1, y1, z1);
    this->b = Point(x2, y2, z2);
    this->c = Point(x3, y3, z3);

    normal = Vector(b.sub(a))
            .cross(Vector(c.sub(a)))
            .norm();
}

float Triangle::getIntersection(const Ray &ray) const {
    float a, b, c, d, e, f, g, h, i, j, k, l;
    float eihf, gfdi, dheg, akjb, jcal, blkc;
    float M, t, beta, gamma;

    a = this->a.x - this->b.x;
    b = this->a.y - this->b.y;
    c = this->a.z - this->b.z;
    d = this->a.x - this->c.x;
    e = this->a.y - this->c.y;
    f = this->a.z - this->c.z;
    g = ray.direction.i;
    h = ray.direction.j;
    i = ray.direction.k;
    j = this->a.x - ray.origin.x;
    k = this->a.y - ray.origin.y;
    l = this->a.z - ray.origin.z;

    eihf = e * i - h * f;
    gfdi = g * f - d * i;
    dheg = d * h - e * g;
    akjb = a * k - j * b;
    jcal = j * c - a * l;
    blkc = b * l - k * c;

    M = a * eihf + b * gfdi + c * dheg;

    t = (- f * akjb - e * jcal - d * blkc) / M;
    if (t < 0)
        return -1;

    gamma = (i * akjb +  h * jcal + g * blkc) / M;
    if (gamma > 1 || gamma < 0)
        return -1;

    beta = (j * eihf + k * gfdi + l * dheg) / M;
    if (beta < 0 || beta > 1 - gamma)
        return -1;

    return t;
}

Vector Triangle::getSurfaceNormal(const Point &p) const {
    return normal;
}

bool Triangle::isFrontFacedTo(const Vector &vec) const {
    return (normal.dot(vec) <= 0);
}
