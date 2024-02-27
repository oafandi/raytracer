#include "../include/Camera.h"
#include "../include/basicTypeDefinition.h"

Camera::Camera()
{
    this->position = Vec3f(0, 0, 0);
    this->gaze = Vec3f(0, 0, -1);
    this->up = Vec3f(0, 1, 0);
    this->near_plane = Vec4f(-1, 1, -1, 1);
    this->near_distance = 1;
    this->image_width = 640;
    this->image_height = 480;
    this->image_name = "out";
}

Camera::Camera(Vec3f position, Vec3f gaze, Vec3f up, Vec4f near_plane, float near_distance, int image_width, int image_height, std::string image_name)
{
    this->position = position;
    this->gaze = gaze;
    this->up = up;
    this->near_plane = near_plane;
    this->near_distance = near_distance;
    this->image_width = image_width;
    this->image_height = image_height;
    this->image_name = image_name;
    this->imageData = new unsigned char[image_width * image_height * 3];
    this->tracingRays = new Ray[image_width * image_height];
}

Camera::~Camera()
{
    if (imageData != nullptr) delete[] imageData;
    if (tracingRays != nullptr) delete[] tracingRays;
}

void Camera::rayTrace(vector<Sphere> &spheres, vector<Face> &faces, Background &background)
{
    int numThreads = 12;
    vector<thread> threads;

    for (int t = 0; t < numThreads; ++ t) {
        threads.push_back(thread([&](int _t) {
            Vec3i color;
            int numOfRays = this->image_height * this->image_width;
            Vec3i colorRay;

            for (size_t i = _t; i < numOfRays; i += numThreads)
            {
                colorRay = tracingRays[i].computeColor(spheres, faces, background).clamp();
                this->imageData[i * 3]     = colorRay.x;
                this->imageData[i * 3 + 1] = colorRay.y;
                this->imageData[i * 3 + 2] = colorRay.z;
            }
        }, t));
    }

    for(auto& t: threads) {
        t.join();
    }
}

void Camera::saveImage()
{
    write_ppm(this->image_name.c_str(), this->imageData, this->image_width, this->image_height);
}

void Camera::computeTracingRays()
{
    Vec3f v = up.normalize();
    Vec3f w = Vec3f(-gaze.x, -gaze.y, -gaze.z).normalize();
    Vec3f u = v.cross(w).normalize();
    for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            Vec3f m = position - w * near_distance;
            Vec3f q = m + u * near_plane.l + v * near_plane.t;
            float s_u = (near_plane.r - near_plane.l) * ((double)(j) + 0.5) / image_width;
            float s_v = (near_plane.t - near_plane.b) * ((double)(i) + 0.5) / image_height;
            Vec3f s = q + u * s_u - v * s_v;
            Vec3f d = s - position;
            d = d.normalize();
            tracingRays[(image_height - i - 1) * image_width + j] = Ray(position, d);
        }
    }
}
