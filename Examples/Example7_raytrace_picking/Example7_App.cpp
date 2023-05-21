#include "Example7_App.h"
#include "rlMath.h"
#include "rlglMeshBank.h"
#include "rlglShaderBank.h"
#include "rlglColors.h"
#include <iostream>

const float MyApp::BOX_WIDTH = 2.5f;

MyApp::MyApp(const std::string& assetDirectory) : BaseApp(assetDirectory){}

void MyApp::prepareAssets() {

    std::string assetDirectory = rlgl::GlobalConfig::assetDirectory;
    assets.mesh.world = rlgl::MeshBank::defaultPlane_textureX10();
    assets.mesh.cubeTex = rlgl::MeshBank::defaultCube_tex();

    assets.material.checker = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/checker_grey.jpg", true);
    assets.material.box = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/box-texture.png", false);

    assets.shader.textured = rlgl::ShaderBank::standardTextureShader();
}


int MyApp::prepareScene() {

    glClearColor(135.f/255.f, 206.f / 255.f, 250.f / 255.f, 1.0f);

    secondaryCam.aspectRatio = windowParams().aspect();
    secondaryCam.position = glm::vec3(70.f);
    secondaryCam.front = glm::vec3(-1.f);
    secondaryCam.upVector = glm::vec3(0.f, 0.f, 1.f);
    secondaryCam.aspectRatio = windowParams().aspect();
    secondaryCam.far = 800.f;

    uiScene.addModel(rlgl::AimCross(rlgl::color::paleVioletRed));    
    scene.addModel(rlgl::CSYSmodel(5.0f, 0.05f, glm::vec3(0.f, 0.f, 0.f)));

    prepareAssets(); 
    createWorld();
    createBoxes();
    return 0;
}

void MyApp::createWorld() {
    objects.worldPlane = new rlgl::Object(assets.mesh.world, assets.shader.textured, assets.material.checker);
    objects.worldPlane->setPosition(glm::vec3(0.f));
    objects.worldPlane->setScale(glm::vec3(100.f));
    scene.addObject(objects.worldPlane);
}


void MyApp::createBoxes() {
    octTree = rl::Octree(rl::OctreeStruct({ 0.f, 0.f, 0.f }, 100.f, 4));

    int             nBoxes = 1e2;
    glm::vec3       boxPos;
    rl::BoundingBox bbox;
    float           boxSize = BOX_WIDTH;

    for (int i = 0; i < nBoxes; i++) {

        float z     = rl::rand(0.f, 20.f);
        float rad   = rl::rand(35.f, 40.f);
        float alpha = rl::rand(0.f, glm::pi<float>() * 2.f);
        boxPos = glm::vec3(rad * glm::cos(alpha), rad * glm::sin(alpha), z);

        objects.cubes.push_back(new rlgl::Object(assets.mesh.cubeTex, assets.shader.textured, assets.material.box));
        objects.cubes[i]->setPosition(boxPos);
        objects.cubes[i]->setScale(glm::vec3(boxSize));
        objects.cubes[i]->setColor(rlgl::color::blue);
        scene.addObject(objects.cubes[i]);

        bbox = rl::BoundingBox::createCubeBoundingBox(boxPos, boxSize);
        rl::OctreeItem* item = octTree.addObject((void*)objects.cubes[i], bbox);
    }
}


static rlgl::Object* lastHitObj = nullptr;
int MyApp::updateScene() {
    camera.position.z = 2.f;
    updateHitTestOctTree();
    updateBoxes();
    return 0;
}

int MyApp::postRender(){
	return 0;
}


void MyApp::processInput(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        activeCamera = &secondaryCam;
        secondaryCam.setFront(glm::vec3(-1.f));
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        activeCamera = &camera;
    }
    BaseApp::processInput(window);
}


/*!
    Performs hit test on objects in OctTree
    - spins and highligh object if hit
*/
void MyApp::updateHitTestOctTree() {
    rl::Ray hitRay(camera.lookVec(), camera.position);
    rlgl::Object* hitObj = nullptr;
    if (octTree.hitTest(hitRay, (void**)&hitObj)) {
        if (lastHitObj && (lastHitObj != hitObj)) lastHitObj->setHighlight(false);

        hitObj->rotate(0.01f, glm::vec3(0.f, 0.f, 1.f));
        hitObj->setHighlight(true);
        lastHitObj = hitObj;
    }
    else {
        if (lastHitObj) lastHitObj->setHighlight(false);
    }
}

void MyApp::updateBoxes() {

    double curTime = glfwGetTime();
    for (rlgl::Object* obj : objects.cubes) {

        obj->translate(glm::vec3(0.1f * glm::sin(curTime * 2.5f), 0.f, 0.f));
        rl::BoundingBox bbox = rl::BoundingBox::createCubeBoundingBox(obj->getPosition(), BOX_WIDTH);

        octTree.moveObject(obj, bbox);
    }
}
