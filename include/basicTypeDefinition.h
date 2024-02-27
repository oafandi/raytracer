#ifndef BASIC_TYPE_DEFINITION_H
#define BASIC_TYPE_DEFINITION_H

#include <string>
#include <vector>
#include <math.h>


using namespace std;

class Vec3i
{
public:
    int x, y, z;
    Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}
    Vec3i() : x(0), y(0), z(0) {}
    int dot(Vec3i v)
    {
        return this->x * v.x + this->y * v.y + this->z * v.y;
    }
    Vec3i cross(Vec3i v)
    {
        return Vec3i(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x);
    }
    Vec3i normalize()
    {
        float length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        return Vec3i(this->x / length, this->y / length, this->z / length);
    }
    Vec3i operator+(const Vec3i &v) const
    {
        return Vec3i(this->x + v.x, this->y + v.y, this->z + v.z);
    }
    Vec3i operator*(const int &f) const
    {
        return Vec3i(this->x * f, this->y * f, this->z * f);
    }
};
class Vec3f
{
public:
    float x, y, z;
    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3f() : x(0), y(0), z(0) {}
    float dot(Vec3f v)
    {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }
    Vec3f cross(Vec3f v)
    {
        return Vec3f(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x);
    }
    Vec3f normalize()
    {
        float length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        return Vec3f(this->x / length, this->y / length, this->z / length);
    }
    Vec3f operator+(const Vec3f &v) const
    {
        return Vec3f(this->x + v.x, this->y + v.y, this->z + v.z);
    }
    Vec3f operator*(const float &f) const
    {
        return Vec3f(this->x * f, this->y * f, this->z * f);
    }
    Vec3f operator*(const Vec3f &v) const // element-wise multiplication
    {
        return Vec3f(x*v.x, y*v.y, z*v.z);
    }
    Vec3f operator/(const Vec3f &v) const // element-wise division
    {
        return Vec3f(x/v.x, y/v.y, z/v.z);
    }
    Vec3f operator/(const float &f) const // division
    {
        return Vec3f(x/f, y/f, z/f);
    }
    Vec3f operator-(const Vec3f &v) const
    {
        return Vec3f(this->x - v.x, this->y - v.y, this->z - v.z);
    }
    float length() const {
        return sqrt(x*x + y*y + z*z);
    }
    Vec3i clamp()
    {
        // this fuction clamps the color values between 0 and 255 and also rounds them to the nearest integer
        return Vec3i(min(255, max(0, (int)round(this->x))), min(255, max(0, (int)round(this->y))), min(255, max(0, (int)round(this->z))));
    }
};


class Vec4f
{
public:
    float l, r, t, b;
    Vec4f(float x, float y, float z, float w) : l(x), r(y), t(z), b(w) {}
    Vec4f() : l(0), r(0), t(0), b(0) {}
};

class Material
{
public: // TODO: make private and write getters
    bool is_mirror;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    Vec3f mirror;
    float phong_exponent;

public:
    Material() {}
    Material(bool is_mirror, Vec3f ambient, Vec3f diffuse, Vec3f specular, Vec3f mirror, float phong_exponent)
    {
        this->is_mirror = is_mirror;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->mirror = mirror;
        this->phong_exponent = phong_exponent;
    }
    ~Material() {}
};
class HitRecord
{
public:
    float t; // negative if no intersection
    Vec3f intersection_point;
    Vec3f normal;
    Material material;
    int material_id;
    HitRecord()
    {
        this->t = -1;
        this->intersection_point = Vec3f(0, 0, 0);
        this->normal = Vec3f(0, 0, 0);
        this->material_id = -1;
    }
    HitRecord(float t, Vec3f intersection_point, Vec3f normal, int material_id)
    {
        this->t = t;
        this->intersection_point = intersection_point;
        this->normal = normal;
        this->material_id = material_id;
    }
};

struct PointLight
{
    Vec3f position;
    Vec3f intensity;
};

class Background
{
private:
    Vec3i backgroundColor;
    Vec3f ambientLight;
    
    vector<PointLight> pointLights;
    vector <Vec3f> vertex_data;
    vector <Material> material;
    int max_recursion_depth;
    float shadow_ray_epsilon;

public:
    // constructors
    Background()
    {
        this->backgroundColor = Vec3i(0, 0, 0);
        this->ambientLight = Vec3f(0, 0, 0);
    }
    Background(Vec3i backgroundColor, Vec3f ambientLight)
    {
        this->backgroundColor = backgroundColor;
        this->ambientLight = ambientLight;
    }
    Background(Vec3i &backgroundColor, Vec3f &ambientLight, vector<PointLight> &pointLights, vector<Vec3f> &vertex_data, int &max_recursion_depth, vector<Material> &materials, float &shadow_ray_epsilon)
    { //TODO: make them references
        this->backgroundColor = backgroundColor;
        this->ambientLight = ambientLight;
        this->pointLights = pointLights;
        this->vertex_data = vertex_data;
        this->max_recursion_depth = max_recursion_depth;
        this->material = materials;
        this->shadow_ray_epsilon = shadow_ray_epsilon;
    }
    // getters
    Vec3i getBackgroundColor() {return this->backgroundColor;}
    Vec3f getAmbientLight() {return this->ambientLight;}
    vector<PointLight> getPointLights() {return this->pointLights;}
    Vec3f getVertex(int id) {return vertex_data[id];}
    int getMaxRecursionDepth() {return max_recursion_depth;}
    Material getMaterial(int id) {return material[id];}
    float getShadowRayEpsilon() {return shadow_ray_epsilon;}
};

class Face
{
public:
    int v0_id;
    int v1_id;
    int v2_id;
    Vec3f normal;
    int material_id;

    // this function calculates the normal of the face
    void calculateNormal(Vec3f v0, Vec3f v1, Vec3f v2)
    {
        Vec3f edge1 = v1 - v0;
        Vec3f edge2 = v2 - v0;
        this->normal = edge1.cross(edge2).normalize();
    }
    Face() {
        v0_id = v1_id = v2_id = 0;
        normal = Vec3f(0, 0, 0);
    }
    Face(int v0_id, int v1_id, int v2_id)
    {
        this->v0_id = v0_id;
        this->v1_id = v1_id;
        this->v2_id = v2_id;
        // calculate the 3 vectors from the vertex data
    }
};

class Object
{
public:
    int material_id;
    int type; // 0 for mesh, 1 for triangle, 2 for sphere
    Material material;

    // all the common functionality between them
};

class Mesh : public Object
{
public:
    vector<Face> faces;
};

class Triangle : public Object
{
public:
    Face face;
};

class Sphere : public Object
{
public:
    int center_vertex_id;
    float radius;
};

#endif