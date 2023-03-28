#include "Example2_App.h"
#include "rlMath.h"
#include <iostream>

const float MyApp::BOX_WIDTH = 2.5f;

MyApp::MyApp(const std::string& assetDirectory) : BaseApp(assetDirectory){}

void MyApp::prepareAssets() {

    //Meshes:
    rlgl::primitive_mesh::cube_tex.initialize();
    rlgl::primitive_mesh::cube.initialize();
    rlgl::primitive_mesh::plane_textureX10.initialize();

    assetIDs.mesh.world = scene.addMesh(&rlgl::primitive_mesh::plane_textureX10);
    assetIDs.mesh.cubeTex = scene.addMesh(&rlgl::primitive_mesh::cube_tex);
    assetIDs.mesh.cube = scene.addMesh(&rlgl::primitive_mesh::cube);

    //Materials (Textures):
    rlgl::Material materialChecker, materialBox;
    materialChecker.initialize(_assetDirectory + "\\textures\\checker_grey.jpg", true);
    materialBox.initialize(_assetDirectory + "\\textures\\box-texture.png", false);

    assetIDs.material.checker = scene.addMaterial(materialChecker);
    assetIDs.material.box = scene.addMaterial(materialBox);

    //Shaders:
    rlgl::StandardShader* shaderTextured = new rlgl::StandardShader();
    shaderTextured->initialize(_assetDirectory + "\\shaders\\object_tex.vs", _assetDirectory + "\\shaders\\object_tex.fs");
    shaderTextured->setInt("textureID", materialChecker.glID); //remove?
    assetIDs.shader.textured = scene.addShader(shaderTextured);

    rlgl::StandardLightShader* shaderTexturedLight = new rlgl::StandardLightShader();
    shaderTexturedLight->initialize(_assetDirectory + "\\shaders\\object_tex_light.vs", _assetDirectory + "\\shaders\\object_tex_light.fs");
    shaderTexturedLight->setInt("textureID", materialChecker.glID);//remove?
    assetIDs.shader.texturedLight = scene.addShader(shaderTexturedLight);

    rlgl::StandardShader* shaderColored = new rlgl::StandardShader();
    shaderColored->initialize(_assetDirectory + "\\shaders\\object_col.vs", _assetDirectory + "\\shaders\\object_col.fs");
    assetIDs.shader.colored = scene.addShader(shaderColored);



    //UI:
    rlgl::primitive_mesh::square.initialize();
    assetIDs.mesh.square = uiScene.addMesh(&rlgl::primitive_mesh::square);

    rlgl::StandardShader* uiShader = new rlgl::StandardShader();
    uiShader->initialize(_assetDirectory + "\\shaders\\ui_element.vs", _assetDirectory + "\\shaders\\ui_element.fs");

    assetIDs.shader.ui = uiScene.addShader(uiShader);
}


int MyApp::prepareScene() {

    glClearColor(135.f/255.f, 206.f / 255.f, 250.f / 255.f, 1.0f);

    prepareAssets();
    createLight();
    createWorld();
    createBoxes();
    createCSYS();
    createUI();
    return 0;
}

void MyApp::createWorld() {
    objects.worldPlane = new rlgl::Object(assetIDs.mesh.world, assetIDs.shader.texturedLight, assetIDs.material.checker);
    objects.worldPlane->setPosition(glm::vec3(0.f));
    objects.worldPlane->setScale(glm::vec3(100.f));
    scene.addObject(objects.worldPlane);
}

void MyApp::createUI() {
    uiObjects.aimCross.push_back(new rlgl::Object(assetIDs.mesh.square, assetIDs.shader.ui, NO_MATERIAL));
    uiObjects.aimCross.push_back(new rlgl::Object(assetIDs.mesh.square, assetIDs.shader.ui, NO_MATERIAL));
    uiObjects.aimCross[0]->setPosition(glm::vec3(0.f, 0.f, 0.2f));
    uiObjects.aimCross[0]->setScale(glm::vec3(0.2f, 0.01f, 1.f));
    uiObjects.aimCross[1]->setPosition(glm::vec3(0.0f, 0.0f, 0.2f));
    uiObjects.aimCross[1]->setScale(glm::vec3(0.01f, 0.2f, 1.f));
    uiObjects.aimCross[0]->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
    uiObjects.aimCross[1]->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
    uiScene.addObject(uiObjects.aimCross[0]);
    uiScene.addObject(uiObjects.aimCross[1]);
}

void MyApp::updateLight() {

    double curTime = glfwGetTime();

    double rad = 15.0f;
    scene.worldEnv.lights[0].pos =
        glm::vec3(10.0f, -5.0f, 5.0f) + glm::vec3(rad * glm::sin(curTime / 0.5f), rad * glm::cos(curTime / 0.5f), 0.f);


    objects.lightBox->setPosition(scene.worldEnv.lights[0].pos);

    //scene.worldEnv.lights[0].ambientIntensity = 0.5f * (glm::sin(curTime/1.0f) + 1.0f);
    //scene.worldEnv.lights[0].color.r = 0.5f * (glm::sin(curTime/2.0f) + 1.0f);
    //scene.worldEnv.lights[0].color.g = 1.0f - scene.worldEnv.lights[0].color.r;
    //scene.worldEnv.lights[0].color.b = 1.0f - scene.worldEnv.lights[0].color.r;
}


void MyApp::createLight() {
    glm::vec3 lightPos(5.f, 5.f, 10.f);
    glm::vec3 lightColor(1.f, 1.f, 1.f);

    scene.worldEnv.lights.push_back({ lightPos, lightColor, 0.2f });
    objects.lightBox = new rlgl::Object(assetIDs.mesh.cube, assetIDs.shader.colored, NO_MATERIAL);
    objects.lightBox->setPosition(lightPos);
    objects.lightBox->setColor(lightColor);
    objects.lightBox->setScale(0.4f);

    scene.addObject(objects.lightBox);
}

void MyApp::createBoxes() {

    float boxSize = BOX_WIDTH;
    glm::vec3 boxPos = glm::vec3(10.f, boxSize, 4.f);

	for (int i = 0; i < 5; i++){
		rlgl::Object* box = new rlgl::Object(
                assetIDs.mesh.cubeTex, 
                assetIDs.shader.texturedLight,
                //(i%2 == 0) ? assetIDs.shader.textured : assetIDs.shader.texturedLight,
                assetIDs.material.box);

		box->setPosition(boxPos);
        box->setScale(boxSize);
		scene.addObject(box);	
        objects.boxes.push_back(box);
		boxPos.y -= boxSize * 1.5f; 
	}

}

void MyApp::createCSYS() {
    float axesL = 5.f;
    float axesW = 0.2f;
    std::vector<glm::vec3> axesDir({ glm::vec3(1.f, 0.f, 0.f),glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f , 0.f, 1.f) });
    std::vector<glm::vec3> axesScales({ glm::vec3(axesL, axesW, axesW),glm::vec3(axesW, axesL, axesW), glm::vec3(axesW, axesW, axesL) });
    std::vector<glm::vec4> axesColor({ glm::vec4(1.f, 0.f, 0.f, 1.f),glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec4(0.f, 0.f, 1.f, 1.f) });
    for (int i = 0; i < axesDir.size(); i++) {
        objects.axes.push_back(new rlgl::Object(assetIDs.mesh.cube, assetIDs.shader.colored, UINT32_MAX));
        objects.axes[i]->setColor(axesColor[i]);
        objects.axes[i]->setPosition(axesDir[i] * axesL / 2.f);
        objects.axes[i]->setScale(axesScales[i]);
        scene.addObject(objects.axes[i]);
    }
}


static rlgl::Object* lastHitObj = nullptr;
int MyApp::updateScene() {
    camera.position.z = 2.f;
    updateBoxes();
    updateLight();
    return 0;
}

int MyApp::postRender(){
	return 0;
}


void MyApp::processInput(GLFWwindow* window) {
    BaseApp::processInput(window);
}

void MyApp::updateBoxes() {

    double curTime = glfwGetTime();
    //for (rlgl::Object* obj : objects.cubes) {
    //
    //    obj->translate(glm::vec3(0.1f * glm::sin(curTime * 2.5f), 0.f, 0.f));
    //    rl::BoundingBox bbox = rl::BoundingBox::createCubeBoundingBox(obj->getPosition(), BOX_WIDTH);
    //
    //    octTree.moveObject(obj, bbox);
    //}

}