#include "Example3_App.h"
#include "rlMath.h"
#include "rlglMeshBank.h"
#include "rlglMeshGenerator.h"
#include "rlglColors.h"
#include <iostream>

const float MyApp::BOX_WIDTH = 2.5f;

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
    auto sphereData1 = std::make_shared<rlgl::MeshVertexData>();
    auto sphereData2 = std::make_shared<rlgl::MeshVertexData>();
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
    rlgl::AimCross aimCross(glm::vec4(rlgl::color::red, 1.f));
    uiScene.addModel(aimCross); 
}

void MyApp::createSkyBox() {
    objects.skyBox = new rlgl::Object(
        assets.mesh.cubeMap,
        assets.shader.skyBox,
        assets.material.skyCubeMap1);
    scene.addObject(objects.skyBox);
}

void MyApp::createLight() {
    glm::vec3 lightPos(0.f, 0.f, 10.f);
    glm::vec3 lightColor(1.f, 1.f, 1.f);

    for(int i = 0; i < 3; i++){

        scene.worldEnv.lights.push_back({ lightPos, lightColor, 1.0f, 1.0f });
        objects.lightBoxes.push_back(new rlgl::Object(assets.mesh.cube, assets.shader.colored, NO_MATERIAL));
        objects.lightBoxes[i]->setPosition(lightPos);
        objects.lightBoxes[i]->setColor(lightColor);
        objects.lightBoxes[i]->setScale(0.4f);

        scene.addObject(objects.lightBoxes[i]);
    }
}

void MyApp::updateLight() {

    double curTime = glfwGetTime();

    double speed[3] = { 0.25f, 2.2f, 4.0f };
    double rad[3] = { 5.f, 7.5f, 10.0f };

    for(int i = 0; i < scene.worldEnv.lights.size(); i++){
        scene.worldEnv.lights[i].pos =
            glm::vec3(0.f, 0.f, 6.0f) + 
            glm::vec3(rad[i] * glm::sin(speed[i] * curTime), rad[i] * glm::cos(speed[i] * curTime), 0.f);
        
        objects.lightBoxes[i]->setPosition(scene.worldEnv.lights[i].pos);
    }
}




void MyApp::createSpheres() {

    int nSpheres = 24;
    float sphereRad = 1.5f;
    float rad = 15.0f;
    float z1 = 3.f;
    float z2 = z1 + 2.1f * sphereRad;
    for (int i = 0; i < nSpheres; i++) {
        
        rlgl::Object* sphere1 = new rlgl::Object(assets.mesh.sphere1, assets.shader.coloredLightMat, assets.material.metalic);
        rlgl::Object* sphere2 = new rlgl::Object(assets.mesh.sphere2, assets.shader.coloredLightMat, assets.material.metalic);

        float ang = glm::two_pi<float>() * (float)i / (float)nSpheres;

        sphere1->setColor(glm::vec3((float)i / (float)nSpheres, 0.2f, 0.3f));
        sphere2->setColor(glm::vec3((float)i / (float)nSpheres, 0.2f, 0.3f));

        sphere1->setPosition(glm::vec3(rad * glm::sin(ang), rad * glm::cos(ang), z1));
        sphere2->setPosition(glm::vec3(rad * glm::sin(ang), rad * glm::cos(ang), z2));
        sphere1->setScale(sphereRad);
        sphere2->setScale(sphereRad);
        
        scene.addObject(sphere1);
        scene.addObject(sphere2);
    }

}

void MyApp::createCSYS() {
    rlgl::CSYSmodel csysModel(5.0f, 0.2f, glm::vec3(0.f, 0.f, 0.f));
    scene.addModel(csysModel);
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