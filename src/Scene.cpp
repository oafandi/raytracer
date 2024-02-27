#include "../include/Scene.h"
#include "../include/Camera.h"

using namespace std;
Scene::Scene()
{
    this->background_color = Vec3i();
    this->shadow_ray_epsilon = 0;
    this->max_recursion_depth = 0;
    this->cameras = std::vector<Camera*>();
    this->ambient_light = Vec3f();
    this->point_lights = std::vector<PointLight>();
    this->materials = std::vector<Material>();
    this->vertex_data = std::vector<Vec3f>();
    this->meshes = std::vector<Mesh>();
    this->triangles = std::vector<Triangle>();
    this->spheres = std::vector<Sphere>();
}

void Scene::renderScene()
{
    Background background(this->background_color, this->ambient_light, this->point_lights, this->vertex_data, this->max_recursion_depth, this->materials, this->shadow_ray_epsilon);
    // this method will go over all the cameras in the scene and render image from each camera
    int size = this->cameras.size();
    // combine all the objects in the scene in a new vector of objects
    vector<Sphere> spheres;
    vector<Face> faces;
    for (size_t i = 0; i < this->meshes.size(); i++) {
        for (int j = 0; j < this->meshes[i].faces.size(); j ++) {
            this->meshes[i].faces[j].material_id = this->meshes[i].material_id;
            faces.push_back((this->meshes[i].faces[j]));
        }
    }
    for (size_t i = 0; i < this->triangles.size(); i++) {
        triangles[i].face.material_id = triangles[i].material_id;
        faces.push_back((this->triangles[i].face));
    }
    for (size_t i = 0; i < this->spheres.size(); i++) {
        spheres.push_back((this->spheres[i]));
    }



    // Ray ray(Vec3f(2, 5, 2), Vec3f(0, -1, 0));
    // Face face(0, 1, 2);
    // float t = ray.calculateFaceIntersection(face, background);
    // std::cout << t << std::endl;
    // return;

    for (size_t i = 0; i < size; i++) {
        cameras[i]->computeTracingRays();
        cameras[i]->rayTrace(spheres, faces, background);
    }
}

void Scene::saveScene()
{
    int size = this->cameras.size();
    for (size_t i = 0; i < size; i++) {
        this->cameras[i]->saveImage();
    }
}
