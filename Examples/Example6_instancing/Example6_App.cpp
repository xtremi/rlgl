#include "Example6_App.h"
#include "rlMath.h"
#include "rlglMeshBank.h"
#include "rlglShaderBank.h"
#include "rlglDefaultMeshes.h"
#include "rlglColors.h"
#include <iostream>

const float MyApp::BOX_WIDTH = 2.5f;

MyApp::MyApp(const std::string& assetDirectory) : BaseApp(assetDirectory){}

void MyApp::prepareAssets() {

    std::string assetDirectory = rlgl::GlobalConfig::assetDirectory;

    cubeTexInstMesh = rlgl::MeshBank::defaultCube_tex();
    
    assets.mesh.world = rlgl::MeshBank::defaultPlane_textureX10();
    assets.mesh.cubeTex = rlgl::MeshBank::defaultCube_tex();
    assets.mesh.cube = rlgl::MeshBank::defaultCube();

    assets.material.checker = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/checker_grey.jpg", true);
    assets.material.box = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/box-texture.png", false);
    assets.material.boxMetal = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/metal-texture-1.jpg", false);

    rlgl::ShaderPtr shaderInst = std::make_shared<rlgl::TextureShader>(
        assetDirectory + "\\shaders\\object_inst.vs", 
        assetDirectory + "\\shaders\\object_inst.fs");
    
    assets.shader.textured = rlgl::ShaderBank::standardTextureShader();
    assets.shader.colored = rlgl::ShaderBank::standardColorShader();
    assets.shader.inst = shaderInst;
}


int MyApp::prepareScene() {

    glClearColor(135.f/255.f, 206.f / 255.f, 250.f / 255.f, 1.0f);
    secondaryCam.aspectRatio = windowParams().aspect();

    secondaryCam.position = glm::vec3(70.f);
    secondaryCam.front = glm::vec3(-1.f);
    secondaryCam.upVector = glm::vec3(0.f, 0.f, 1.f);
    secondaryCam.aspectRatio = windowParams().aspect();
    secondaryCam.far = 800.f;

    rlgl::AimCross aimCross(rlgl::color::red);
    uiScene.addModel(aimCross);
    rlgl::CSYSmodel csysModel(5.0f, 0.05f, glm::vec3(0.f, 0.f, 0.f));
    scene.addModel(csysModel);

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
    glm::vec3 boxPos;

    int nInstances = 1e4;
    float radiusMin = 30.0f;
    float radiusMax = 50.0f;

    for (int i = 0; i < nInstances; i++) {
        float rad   = rl::rand(radiusMin, radiusMax);
        float alpha = rl::rand(0.f, glm::pi<float>() * 2.f);
		float x     = rl::rand(0.f, 1.f);
		float theta = glm::acos(2.f*x - 1.f);   //cos(0.0 -> 1.0f)
        boxPos = glm::vec3(
            rad * glm::sin(alpha) * glm::cos(theta), 
            rad * glm::sin(alpha) * glm::sin(theta), 
            rad * glm::cos(alpha));

        glm::mat4 tMat = glm::translate(glm::mat4(1.f), boxPos);
        glm::mat4 sMat = glm::scale(glm::mat4(1.f), glm::vec3(0.25f));
        glm::mat4 mMat = tMat * sMat;
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                cubeTexInstMesh->instances.data.push_back(mMat[j][k]);
            }
        }
    }
    cubeTexInstMesh->hasInstances = true;
    cubeTexInstMesh->initialize();
    assets.mesh.cubeInst = cubeTexInstMesh;

    objects.instObj = new rlgl::Object(assets.mesh.cubeInst, assets.shader.inst, assets.material.boxMetal);
    objects.instObj->setNinstances(nInstances);
    scene.addObject(objects.instObj);

}

    
int MyApp::updateScene() {
    camera.position.z = 2.f;
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


