#ifndef SCENE_H
#define SCENE_H

#include "basicTypeDefinition.h"
#include "Camera.h"

#include <string>

#include "tinyxml2.h"
#include <sstream>
#include <stdexcept>

class Scene
{
public:
    Scene();
    ~Scene() {
        for (int i = 0; i < cameras.size(); ++ i) {
            delete cameras[i];
        }
    }
    void loadScene(const std::string &filename);
    void renderScene();
    void saveScene();

private:
    Vec3i background_color;
    float shadow_ray_epsilon;
    int max_recursion_depth;
    std::vector<Camera*> cameras;
    Vec3f ambient_light;
    std::vector<PointLight> point_lights;
    std::vector<Material> materials;
    std::vector<Vec3f> vertex_data;
    std::vector<Mesh> meshes;
    std::vector<Triangle> triangles;
    std::vector<Sphere> spheres;
};

#endif