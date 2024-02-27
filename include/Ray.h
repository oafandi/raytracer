#ifndef RAY_BETTER_H
#define RAY_BETTER_H

#include "basicTypeDefinition.h"
#include <iostream>

class Ray
{
private:
    Vec3f origin;
    Vec3f direction;
    int depth;
    HitRecord hit_record;

public:
    Ray() {}
    Ray(Vec3f origin, Vec3f direction);
    ~Ray() {}
    Vec3f getDirection() {return direction;}
    Vec3f getOrigin() {return origin;}
    bool closestIntersection(vector<Sphere> &spheres, vector<Face> &faces, Background &background);
    float calculateSphereIntersection(Sphere &sphere, Background &background);
    float calculateFaceIntersection(Face &face, Background &background);
    bool isInShadow (const PointLight &light, vector<Sphere> &spheres, vector<Face> &faces, Background &background); 
    Vec3f computeColor(vector<Sphere> &spheres, vector<Face> &faces, Background &background);
    Vec3f applyShading(vector<Sphere> &spheres, vector<Face> &faces, Background &background); 
};

#endif