#include <iostream>
#include "../include/basicTypeDefinition.h"
#include "../include/Ray.h"
#include "../include/Camera.h"
#include "../include/Scene.h"


using namespace std;

int main(int argc, char *argv[])
{
    Scene scene;
    scene.loadScene(argv[1]);
    scene.renderScene();
    scene.saveScene();
    return 0;
}