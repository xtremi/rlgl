#include "Example1_App.h"
#include "rlMath.h"
#include "rlglBaseModel.h"
#include "rlglMeshBank.h"
#include "rlglShaderBank.h"
#include "rlglMeshGenerator.h"
#include <iostream>
const float MyApp::BOX_WIDTH = 2.5f;

MyApp::MyApp(const std::string& assetDirectory) : BaseApp(assetDirectory){}

void MyApp::prepareAssets() {

    std::string assetDirectory = rlgl::GlobalConfig::assetDirectory;

    //Meshes:
    assets.mesh.world = rlgl::MeshBank::defaultPlane_textureX10();
    assets.mesh.cubeTex = rlgl::MeshBank::defaultCube_tex();
    assets.mesh.cube = rlgl::MeshBank::defaultCube();

    //Shaders:
    assets.shader.textured = rlgl::ShaderBank::standardTextureShader();
    assets.shader.colored = rlgl::ShaderBank::standardColorShader();

    //Materials (Textures):
    assets.material.checker = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/checker_grey.jpg", true);
    assets.material.box = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/box-texture.png", false);

    //UI:
    //Meshes:
    assets.mesh.square = rlgl::MeshBank::defaultSquare();

    //Shaders:
    assets.shader.ui = rlgl::ShaderBank::standardUIcolShader();
}


int MyApp::prepareScene() {
    glClearColor(135.f/255.f, 206.f / 255.f, 250.f / 255.f, 1.0f);
    prepareAssets();
    createWorld();
    createBox();
    createCSYS();
    createUI();
    return 0;
}

void MyApp::createWorld() {
    objects.worldPlane = new rlgl::Object(assets.mesh.world, assets.shader.textured, assets.material.checker);
    objects.worldPlane->setPosition(glm::vec3(0.f));
    objects.worldPlane->setScale(glm::vec3(100.f));
    scene.addObject(objects.worldPlane);
}

void MyApp::createUI() {
    uiObjects.aimCross.push_back(new rlgl::Object(assets.mesh.square, assets.shader.ui, NO_MATERIAL));
    uiObjects.aimCross.push_back(new rlgl::Object(assets.mesh.square, assets.shader.ui, NO_MATERIAL));
    uiObjects.aimCross[0]->setPosition(glm::vec3(0.f, 0.f, 0.2f));
    uiObjects.aimCross[0]->setScale(glm::vec3(0.2f, 0.01f, 1.f));
    uiObjects.aimCross[1]->setPosition(glm::vec3(0.0f, 0.0f, 0.2f));
    uiObjects.aimCross[1]->setScale(glm::vec3(0.01f, 0.2f, 1.f));
    uiObjects.aimCross[0]->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
    uiObjects.aimCross[1]->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
    uiScene.addObject(uiObjects.aimCross[0]);
    uiScene.addObject(uiObjects.aimCross[1]);
}

void MyApp::createBox() {
    glm::vec3 boxPos = glm::vec3(BOX_WIDTH);
    objects.cube = new rlgl::Object(assets.mesh.cubeTex, assets.shader.textured, assets.material.box);
    objects.cube->setPosition(boxPos);
    objects.cube->setScale(BOX_WIDTH);
    scene.addObject(objects.cube);
}

void MyApp::createCSYS() {
    rlgl::CSYSmodel csysModel(5.0f, 0.05f, glm::vec3(0.f, 0.f, 0.f));
    scene.addModel(csysModel);
}


int MyApp::updateScene() {
    camera.position.z = 2.f;

    double curTime = glfwGetTime();
    objects.cube->rotate(0.01f, glm::vec3(1.0f, 1.0f, 0.0f));
    
    return 0;
}
