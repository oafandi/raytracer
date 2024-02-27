#ifndef CAMERA_H
#define CAMERA_H

#include "basicTypeDefinition.h"
#include "ppm.h"
#include "Ray.h"
#include <string>
#include <thread>

using namespace std;
class Camera
{
public:
    Camera();
    Camera(Vec3f position, Vec3f gaze, Vec3f up, Vec4f near_plane, float near_distance, int image_width, int image_height, std::string image_name);
    ~Camera();
    void rayTrace(vector<Sphere> &spheres, vector<Face> &faces, Background &background);
    void saveImage();
    void computeTracingRays();

private:
    Vec3f position;                // this is the origin of the camera in the world coordinate system (x,y,z)
    Vec3f gaze;                    // this is the gaze direction of the camera (-w)
    Vec3f up;                      // this is the up vector of the camera (v)
    Vec4f near_plane;              // this is the near plane of the camera (l,r,b,t)
    float near_distance;           // this is the distance between the camera and the near plane (d)
    int image_width, image_height; // this is the width and height of the image that will be produced by the camera (nx,ny)
    std::string image_name;
    unsigned char *imageData = nullptr;
    Ray *tracingRays = nullptr;    // this is the array of rays that will be used to trace the scene from the camera (r)
};

#endif