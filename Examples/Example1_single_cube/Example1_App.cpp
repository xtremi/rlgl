#include "Example1_App.h"
#include "rlMath.h"
#include <iostream>

const float MyApp::BOX_WIDTH = 2.5f;

MyApp::MyApp(const std::string& assetDirectory) : BaseApp(assetDirectory){}

void MyApp::prepareAssets() {

    rlgl::primitive_mesh::cube_tex.initialize();
    rlgl::primitive_mesh::cube.initialize();
    rlgl::primitive_mesh::plane_textureX10.initialize();

    assets.mesh.world = scene.addMesh(&rlgl::primitive_mesh::plane_textureX10);
    assets.mesh.cubeTex = scene.addMesh(&rlgl::primitive_mesh::cube_tex);
    assets.mesh.cube = scene.addMesh(&rlgl::primitive_mesh::cube);

    rlgl::Material materialChecker, materialBox;
    materialChecker.initialize(_assetDirectory + "\\textures\\checker_grey.jpg", true);
    materialBox.initialize(_assetDirectory + "\\textures\\box-texture.png", false);

    assets.material.checker = scene.addMaterial(materialChecker);
    assets.material.box = scene.addMaterial(materialBox);

    rlgl::Shader shaderTextured, shaderColored, shaderInst, shaderGrass;
    shaderTextured.initialize(_assetDirectory + "\\shaders\\object.vs", _assetDirectory + "\\shaders\\object.fs");
    shaderTextured.setInt("textureID", materialChecker.glID);
    shaderColored.initialize(_assetDirectory + "\\shaders\\object_col.vs", _assetDirectory + "\\shaders\\object_col.fs");

    assets.shader.textured = scene.addShader(shaderTextured);
    assets.shader.colored = scene.addShader(shaderColored);


    //UI:
    rlgl::primitive_mesh::square.initialize();
    assets.mesh.square = uiScene.addMesh(&rlgl::primitive_mesh::square);

    rlgl::Shader uiShader;
    uiShader.initialize(_assetDirectory + "\\shaders\\ui_element.vs", _assetDirectory + "\\shaders\\ui_element.fs");

    assets.shader.ui = uiScene.addShader(uiShader);
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

    glm::vec3 boxPos = glm::vec3(10.f, 10.f, 5.f);
    float boxSize = BOX_WIDTH/5.f;

    objects.cube = new rlgl::Object(assets.mesh.cubeTex, assets.shader.textured, assets.material.box);
    objects.cube->setPosition(boxPos);
    scene.addObject(objects.cube);
}

void MyApp::createCSYS() {
    float axesL = 5.f;
    float axesW = 0.2f;
    std::vector<glm::vec3> axesDir({ glm::vec3(1.f, 0.f, 0.f),glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f , 0.f, 1.f) });
    std::vector<glm::vec3> axesScales({ glm::vec3(axesL, axesW, axesW),glm::vec3(axesW, axesL, axesW), glm::vec3(axesW, axesW, axesL) });
    std::vector<glm::vec4> axesColor({ glm::vec4(1.f, 0.f, 0.f, 1.f),glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec4(0.f, 0.f, 1.f, 1.f) });
    for (int i = 0; i < axesDir.size(); i++) {
        objects.axes.push_back(new rlgl::Object(assets.mesh.cube, assets.shader.colored, UINT32_MAX));
        objects.axes[i]->setColor(axesColor[i]);
        objects.axes[i]->setPosition(axesDir[i] * axesL / 2.f);
        objects.axes[i]->setScale(axesScales[i]);
        scene.addObject(objects.axes[i]);
    }
}


static rlgl::Object* lastHitObj = nullptr;
int MyApp::updateScene() {
    camera.position.z = 2.f;
    updateBox();
    return 0;
}

int MyApp::postRender(){
	return 0;
}


void MyApp::processInput(GLFWwindow* window) {
    BaseApp::processInput(window);
}

void MyApp::updateBox() {

    double curTime = glfwGetTime();
    //for (rlgl::Object* obj : objects.cubes) {
    //
    //    obj->translate(glm::vec3(0.1f * glm::sin(curTime * 2.5f), 0.f, 0.f));
    //    rl::BoundingBox bbox = rl::BoundingBox::createCubeBoundingBox(obj->getPosition(), BOX_WIDTH);
    //
    //    octTree.moveObject(obj, bbox);
    //}

}