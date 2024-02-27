#include "../include/basicTypeDefinition.h"
#include "../include/Ray.h"

#define EPS 1e-10

Ray::Ray(Vec3f origin, Vec3f direction)
{
    this->origin = origin;
    this->direction = direction;
    this->depth = 0;
    this->hit_record.t = -1;
    this->hit_record.material_id = -1;
    this->hit_record.intersection_point = Vec3f(0, 0, 0);
    this->hit_record.normal = Vec3f(0, 0, 0);
}

bool Ray::closestIntersection(vector<Sphere> &spheres, vector<Face> &faces, Background &background) {
    for (int i = 0; i < spheres.size(); i ++) {
        // update ray's hit record
        float t = calculateSphereIntersection(spheres[i], background);
        if (t > 0 and (hit_record.t < 0 or t < hit_record.t)) {
            hit_record.t = t;
            hit_record.material_id = spheres[i].material_id;
            hit_record.material = background.getMaterial(hit_record.material_id-1);
            hit_record.intersection_point = origin + direction * hit_record.t;
            hit_record.normal = 
                (hit_record.intersection_point - background.getVertex(spheres[i].center_vertex_id-1)).normalize();
        }
    }
    for (int i = 0; i < faces.size(); i ++) {
        // update ray's hit record
        float t = calculateFaceIntersection(faces[i], background);
        if (t > 0 and (hit_record.t < 0 or t < hit_record.t)) {
            hit_record.t = t;
            hit_record.material_id = faces[i].material_id;
            hit_record.material = background.getMaterial(hit_record.material_id-1);
            hit_record.normal = faces[i].normal;
        }
    }
    if (hit_record.t > 0) {
        hit_record.intersection_point = origin + direction * hit_record.t;
        return true;
    }
    return false;
}

float Ray::calculateFaceIntersection(Face &face, Background &background)
{
    Vec3f vertex1 = background.getVertex(face.v0_id-1);
    Vec3f vertex2 = background.getVertex(face.v1_id-1);
    Vec3f vertex3 = background.getVertex(face.v2_id-1);
    face.calculateNormal(vertex1, vertex2, vertex3);
    Vec3f edge1 = vertex2 - vertex1;
    Vec3f edge2 = vertex3 - vertex1;
    Vec3f triangleNormal = edge1.cross(edge2);
    Vec3f h = direction.cross(edge2);
    float determinantA = edge1.dot(h);
    float t = -1;
    if (determinantA > -EPS and determinantA < EPS) return t;
    float f = 1.0 / determinantA;
    Vec3f s = (origin - vertex1);
    float u = f * s.dot(h);
    if (u < 0.0 or u > 1.0) return t;
    Vec3f q = s.cross(edge1);
    float v = f * direction.dot(q);
    if (v < 0.0 or u + v > 1.0) return t;
    t = f * edge2.dot(q);
    if (t < 1e-6) t = -1;
    return t;
}

float Ray::calculateSphereIntersection(Sphere &sphere, Background &background)
{
    Vec3f center = background.getVertex(sphere.center_vertex_id-1);
    float t = -1;
    // calculate the coefficients for the quadratic equation
    float a = direction.dot(direction);
    float b = direction.dot(origin - center);
    float c = (origin - center).dot(origin - center) - sphere.radius*sphere.radius;
    
    float discriminant = b*b - a*c;

    if (discriminant < 0) {
        return t;
    }

    float t1 = (-b - sqrt(discriminant)) / (a);
    float t2 = (-b + sqrt(discriminant)) / (a);

    if (t1 >= 0 and t1 < t2) t = t1;
    else if (t2 >= 0) t = t2;

    return t;
}

bool Ray::isInShadow (const PointLight &light, vector<Sphere> &spheres, vector<Face> &faces, Background &background) {
    Vec3f shadowRayDirection = (light.position - hit_record.intersection_point).normalize();
    Ray shadowRay(hit_record.intersection_point + hit_record.normal*(background.getShadowRayEpsilon()), shadowRayDirection);

    // here we want to proceed through all of the objects and find whether there is an intersection or not
    if (shadowRay.closestIntersection(spheres, faces, background)) { // there is an object in the direction of the ray
        float tLight = (light.position.x - shadowRay.origin.x) / shadowRay.direction.x;
        if (shadowRay.hit_record.t < tLight) return true; // the object is between the intersection point and light
    }
    
    // no intersection with objects; the point is in the light
    return false;
}

Vec3f Ray::computeColor(vector<Sphere> &spheres, vector<Face> &faces, Background &background)
{
    if (depth > background.getMaxRecursionDepth()) { // max depth exceeded
        return Vec3f(0, 0, 0);
    }
    if (closestIntersection(spheres, faces, background)) { // find the color at the closest hit point
        return applyShading(spheres, faces, background);
    }
    else if (depth == 0) { // no intersection for the primary ray
        return Vec3f(background.getBackgroundColor().x, background.getBackgroundColor().y, background.getBackgroundColor().z);
    }
    else { // avoid repeated addition of the background color for reflected rays
        return Vec3f(0, 0, 0);
    }
}

Vec3f Ray::applyShading(vector<Sphere> &spheres, vector<Face> &faces, Background &background) {
    Vec3f color = background.getAmbientLight() * hit_record.material.ambient;
    if (hit_record.material.is_mirror) {
        Ray reflectionRay(hit_record.intersection_point + hit_record.normal*(background.getShadowRayEpsilon()), (direction - hit_record.normal * 2.0f * direction.dot(hit_record.normal)).normalize());
        reflectionRay.depth = depth + 1;
        color = color + reflectionRay.computeColor(spheres, faces, background) * hit_record.material.mirror;
    }
    int numOfLights = background.getPointLights().size();
    for (int i = 0; i < numOfLights; i ++) {
        if (isInShadow(background.getPointLights()[i], spheres, faces, background)) {
            continue;
        }
        Vec3f lightDirection = background.getPointLights()[i].position - hit_record.intersection_point;
        lightDirection = lightDirection.normalize();
        float diffuse = lightDirection.dot(hit_record.normal);
        if (diffuse < 0) {
            diffuse = 0;
        }
        Vec3f specularDirection = lightDirection - direction;
        specularDirection = specularDirection.normalize();
        float specular = specularDirection.dot(hit_record.normal);
        if (specular < 0) {
            specular = 0;
        }
        specular = pow(specular, hit_record.material.phong_exponent);

        float lightDistance = (background.getPointLights()[i].position - hit_record.intersection_point).length();
        color = color + background.getPointLights()[i].intensity / (lightDistance*lightDistance) * (hit_record.material.diffuse * diffuse + hit_record.material.specular * specular);
    }
    return color;
}