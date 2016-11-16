//
// Created by bahuljain on 10/6/16.
//

#include "include/Triangle.h"

Triangle::Triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3) {
    this->isInMesh = false;
    this->p1 = Point(x1, y1, z1);
    this->p2 = Point(x2, y2, z2);
    this->p3 = Point(x3, y3, z3);

    normal = Vector(p2.sub(p1))
            .cross(Vector(p3.sub(p1)))
            .norm();

    _a = this->p1.x - this->p2.x;
    _b = this->p1.y - this->p2.y;
    _c = this->p1.z - this->p2.z;
    _d = this->p1.x - this->p3.x;
    _e = this->p1.y - this->p3.y;
    _f = this->p1.z - this->p3.z;

    float x_min, x_max, y_min, y_max, z_min, z_max;

    x_min = fminf(p1.x, fminf(p2.x, p3.x));
    x_max = fmaxf(p1.x, fmaxf(p2.x, p3.x));
    y_min = fminf(p1.y, fminf(p2.y, p3.y));
    y_max = fmaxf(p1.y, fmaxf(p2.y, p3.y));
    z_min = fminf(p1.z, fminf(p2.z, p3.z));
    z_max = fmaxf(p1.z, fmaxf(p2.z, p3.z));

    this->bbox = new BoundingBox(x_min, x_max, y_min, y_max, z_min, z_max);
}

float Triangle::getIntersection(const Ray &ray) const {
    float g, h, i, j, k, l;
    float eihf, gfdi, dheg, akjb, jcal, blkc;
    float M, t, beta, gamma;

    g = ray.direction.i;
    h = ray.direction.j;
    i = ray.direction.k;
    j = this->p1.x - ray.origin.x;
    k = this->p1.y - ray.origin.y;
    l = this->p1.z - ray.origin.z;

    eihf = _e * i - h * _f;
    gfdi = g * _f - _d * i;
    dheg = _d * h - _e * g;
    akjb = _a * k - j * _b;
    jcal = j * _c - _a * l;
    blkc = _b * l - k * _c;

    M = _a * eihf + _b * gfdi + _c * dheg;

    t = (-_f * akjb - _e * jcal - _d * blkc) / M;
    if (t < 0)
        return -1;

    gamma = (i * akjb + h * jcal + g * blkc) / M;
    if (gamma > 1 || gamma < 0)
        return -1;

    beta = (j * eihf + k * gfdi + l * dheg) / M;
    if (beta < 0 || beta > 1 - gamma)
        return -1;

    return t;
}

Vector Triangle::getSurfaceNormal(const Point &p) const {
    if (!this->isInMesh)
        return normal;

    float a, b, c, d, e, f, g, h, i, j, k, l;
    float eihf, gfdi, dheg, akjb, jcal, blkc;
    float M, alpha, beta, gamma;

    a = p1.x;
    b = p1.y;
    c = p1.z;
    d = p2.x;
    e = p2.y;
    f = p2.z;
    g = p3.x;
    h = p3.y;
    i = p3.z;
    j = p.x;
    k = p.y;
    l = p.z;

    eihf = e * i - h * f;
    gfdi = g * f - d * i;
    dheg = d * h - e * g;
    akjb = a * k - j * b;
    jcal = j * c - a * l;
    blkc = b * l - k * c;

    M = a * eihf + b * gfdi + c * dheg;
    alpha = (j * eihf + k * gfdi + l * dheg) / M;
    beta = (i * akjb + h * jcal + g * blkc) / M;
    gamma = (-f * akjb - e * jcal - d * blkc) / M;

    return n1.times(alpha)
            .plus(n2.times(beta))
            .plus(n3.times(gamma))
            .norm();
}

bool Triangle::isFrontFacedTo(const Ray &ray) const {
    return (normal.dot(ray.direction) <= 0);
}
