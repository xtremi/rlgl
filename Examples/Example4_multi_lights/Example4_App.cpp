#include "Example4_App.h"
#include "rlMath.h"
#include "rlglMeshBank.h"
#include "rlglMeshGenerator.h"
#include <iostream>

MyApp::MyApp(const std::string& assetDirectory) : BaseApp(assetDirectory){}

void MyApp::prepareAssets() {

    std::string assetDirectory = rlgl::GlobalConfig::assetDirectory;

    //Meshes:
    assets.mesh.world = rlgl::MeshBank::defaultPlane_textureX10();
    assets.mesh.cube = rlgl::MeshBank::defaultCube();
    assets.mesh.cubeMap = rlgl::MeshBank::defaultCubeMap();

    //Shaders:
    assets.shader.textured = std::make_shared<rlgl::TextureShader>(
        assetDirectory + "/shaders/object_tex.vs",
        assetDirectory + "/shaders/object_tex.fs");
    assets.shader.texturedLight = std::make_shared<rlgl::TextureLightShader>(
        assetDirectory + "/shaders/object_tex_light.vs",
        assetDirectory + "/shaders/object_tex_light.fs");
    assets.shader.texturedLightMat = std::make_shared<rlgl::TextureLightMaterialShader>(
        assetDirectory + "/shaders/object_tex_light_mat.vs",
        assetDirectory + "/shaders/object_tex_light_mat.fs");
    assets.shader.coloredLightMat = std::make_shared<rlgl::LightMaterialShader>(
        assetDirectory + "/shaders/object_col_light_mat.vs",
        assetDirectory + "/shaders/object_col_light_mat.fs");
    assets.shader.colored = std::make_shared<rlgl::StandardShader>(
        assetDirectory + "/shaders/object_col.vs",
        assetDirectory + "/shaders/object_col.fs");

    assets.shader.skyBox = std::make_shared<rlgl::CubeMapShader>(
        assetDirectory + "/shaders/sky_cubemap.vs",
        assetDirectory + "/shaders/sky_cubemap.fs");

    //Materials (Textures):
    //http://devernay.free.fr/cours/opengl/materials.html
    assets.material.checker = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/checker_grey.jpg", true);
    assets.material.metalic = std::make_shared<rlgl::TextureLightPropMaterial>(
        assetDirectory + "/textures/metal-texture-1.jpg", false, rlgl::LightProperties::mettalic());
    assets.material.skyCubeMap1 = std::make_shared<rlgl::TexturedCubeMapMaterial>(
        std::vector<std::string>({
            assetDirectory + "/textures/skybox/right.jpg",
            assetDirectory + "/textures/skybox/left.jpg",
            assetDirectory + "/textures/skybox/top.jpg",
            assetDirectory + "/textures/skybox/bottom.jpg",
            assetDirectory + "/textures/skybox/front.jpg",
            assetDirectory + "/textures/skybox/back.jpg",
            })
        );


    rlgl::MeshFactory meshGen;
    std::shared_ptr<rlgl::MeshVertexData> sphereData1 = std::make_shared<rlgl::MeshVertexData>();
    std::shared_ptr<rlgl::MeshVertexData> sphereData2 = std::make_shared<rlgl::MeshVertexData>();
    meshGen.generateSphere(sphereData1, rl::geom::Sphere(1.0f), 32, false);
    meshGen.generateSphere(sphereData2, rl::geom::Sphere(1.0f), 32, true);

    assets.mesh.sphere1 = std::make_shared<rlgl::Mesh>(
        rlgl::GLBuffer<float>(sphereData1->vertices),
        true, false, false);
    assets.mesh.sphere1->initialize();
    
    assets.mesh.sphere2 = std::make_shared<rlgl::Mesh>(
        rlgl::GLBuffer<float>(sphereData2->vertices),
        rlgl::GLBuffer<unsigned int>(sphereData2->indices),
        true, false, false);
    assets.mesh.sphere2->initialize();


    //############## UI ##########################################
 
    //Meshes:
    assets.mesh.square = rlgl::MeshBank::defaultSquare();

    //Shaders:
    assets.shader.ui = std::make_shared<rlgl::StandardShader>(
        assetDirectory + "/shaders/ui_element.vs", 
        assetDirectory + "/shaders/ui_element.fs");
}


int MyApp::prepareScene() {

    glClearColor(135.f/255.f, 206.f / 255.f, 250.f / 255.f, 1.0f);

    prepareAssets();        
    createLight();
    createWorld();
    createSpheres();
    createCSYS();
    createUI();
    createSkyBox();
    return 0;
}

int MyApp::updateScene() {
    //camera.position.z = 2.f;
    updateBoxes();
    updateLight();
    return 0;
}

void MyApp::createWorld() {
    objects.worldPlane = new rlgl::Object(
        assets.mesh.world, 
        assets.shader.texturedLight, 
        assets.material.checker);

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

void MyApp::createSkyBox() {
    objects.skyBox = new rlgl::Object(
        assets.mesh.cubeMap,
        assets.shader.skyBox,
        assets.material.skyCubeMap1);
    scene.addObject(objects.skyBox);
}

void MyApp::createLight() {
    glm::vec3 lightPos(0.f, 0.f, 0.f);
    glm::vec3 lightColors[3] = {
        glm::vec3(1.0f, 0.1f, 0.1f),
        glm::vec3(0.0f, 1.0f, 0.1f),
        glm::vec3(0.0f, 0.1f, 1.0f)
    };

    for(int i = 0; i < 3; i++){

        scene.worldEnv.lights.push_back({ lightPos, lightColors[i], 0.3f, 0.3f});
        objects.lightBoxes.push_back(new rlgl::Object(assets.mesh.cube, assets.shader.colored, NO_MATERIAL));
        objects.lightBoxes[i]->setPosition(lightPos);
        objects.lightBoxes[i]->setColor(lightColors[i]);
        objects.lightBoxes[i]->setScale(0.4f);

        scene.addObject(objects.lightBoxes[i]);
    }
}

void MyApp::updateLight() {

    double curTime = glfwGetTime();

    double speed[3] = { 0.25f, 2.2f, 4.0f };
    double rad[3] = { 2.5f, 4.5f, 6.0f };

    for(int i = 0; i < scene.worldEnv.lights.size(); i++){
        scene.worldEnv.lights[i].pos =
            glm::vec3(5.f, 0.f, 5.0f) + 
            glm::vec3(rad[i] * glm::sin(speed[i] * curTime), rad[i] * glm::cos(speed[i] * curTime), 0.f);
        
        objects.lightBoxes[i]->setPosition(scene.worldEnv.lights[i].pos);
    }
}




void MyApp::createSpheres() {
    rlgl::Object* sphere1 = new rlgl::Object(assets.mesh.sphere1, assets.shader.coloredLightMat, assets.material.metalic);
    sphere1->setColor(glm::vec3(0.5f, 0.5f, 0.5f));
    sphere1->setPosition(glm::vec3(5.f, 0.f, 5.f));
    sphere1->setScale(2.f);
    scene.addObject(sphere1);
}

void MyApp::createCSYS() {
    float axesL = 5.f;
    float axesW = 0.2f;
    std::vector<glm::vec3> axesDir({ glm::vec3(1.f, 0.f, 0.f),glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f , 0.f, 1.f) });
    std::vector<glm::vec3> axesScales({ glm::vec3(axesL, axesW, axesW),glm::vec3(axesW, axesL, axesW), glm::vec3(axesW, axesW, axesL) });
    std::vector<glm::vec4> axesColor({ glm::vec4(1.f, 0.f, 0.f, 1.f),glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec4(0.f, 0.f, 1.f, 1.f) });
    for (int i = 0; i < axesDir.size(); i++) {
        objects.axes.push_back(new rlgl::Object(assets.mesh.cube, assets.shader.colored, NO_MATERIAL));
        objects.axes[i]->setColor(axesColor[i]);
        objects.axes[i]->setPosition(axesDir[i] * axesL / 2.f);
        objects.axes[i]->setScale(axesScales[i]);
        scene.addObject(objects.axes[i]);
    }
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