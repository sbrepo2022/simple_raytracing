#ifndef SBRAYTRACING_H
#define SBRAYTRACING_H

#include <QDebug>

#define QT_SAVE

#ifdef QT_SAVE
#include <QImage>
#endif

#include <limits>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include "sbgeometry.h"

static SBVector3 bg_color(0.8f, 0.8f, 1.0f);

class Material {
public:
    SBVector3 diffuse_color;
    SBVector2 albedo;
    float spec_exponent;
    float reflection;

    Material() {}
    void setDiffuseColor(SBVector3 diffuse_color) {this->diffuse_color = diffuse_color;}
};

class Sphere {
public:
    SBVector3 center;
    float radius;
    Material mat;

    Sphere() {}
    Sphere(SBVector3 center, float radius, Material mat) : center(center) , radius(radius), mat(mat) {}
    bool rayCast(const SBVector3 &orig, const SBVector3 &dir, float *length);
};

class Light {
public:
    SBVector3 position;
    float bright;
};

class Scene {
public:
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
};

void render(bool camera_type, Scene &scene, int antialiasing);

bool castRay(Scene &scene, const SBVector3 &orig, const SBVector3 &dir, Material *mat, int depth, int cur_depth);

SBVector3 reflectRay(const SBVector3 &dir, SBVector3 &normal);

SBVector3 mix(const SBVector3 &v1, const SBVector3 &v2, float factor);

#endif // SBRAYTRACING_H
