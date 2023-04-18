#include "ExampleQT1_App.h"
#include "rlMath.h"
#include "rlglMeshBank.h"
#include "rlglMeshGenerator.h"
#include <iostream>
#include <QKeyEvent>
#include <QOpenGLContext>

const float MyApp::BOX_WIDTH = 2.5f;

MyApp::MyApp(const std::string& assetDirectory) : BaseApp(assetDirectory, false),QObject(){

}

void MyApp::prepareAssets() {

    std::string assetDirectory = rlgl::GlobalConfig::assetDirectory;

    //Meshes:
    assets.mesh.world = rlgl::MeshBank::defaultPlane_textureX10();
    assets.mesh.cubeTex = rlgl::MeshBank::defaultCube_tex();
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

    assets.material.jade = std::make_shared<rlgl::LightPropMaterial>(rlgl::LightProperties::jade());
    assets.material.gold = std::make_shared<rlgl::LightPropMaterial>(rlgl::LightProperties::gold());

    assets.material.box = std::make_shared<rlgl::TextureLightPropMaterial>(
        assetDirectory + "/textures/box-texture.png", false, rlgl::LightProperties::standard());

    assets.material.boxJade = std::make_shared<rlgl::TextureLightPropMaterial>(
        assetDirectory + "/textures/box-texture.png", false, rlgl::LightProperties::jade());

    assets.material.boxGold = std::make_shared<rlgl::TextureLightPropMaterial>(
        assetDirectory + "/textures/box-texture.png", false, rlgl::LightProperties::gold());

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
    createBoxes();
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


void MyApp::updateLight() {

    double curTime = glfwGetTime();

    double speed = 2.2f;
    double rad = 15.0f;
    scene.worldEnv.lights[0].pos =
        glm::vec3(10.0f, -5.0f, 15.0f) + 
        glm::vec3(rad*0.5f * glm::sin(speed * curTime), rad * glm::cos(speed * curTime), 0.f);


    objects.lightBox->setPosition(scene.worldEnv.lights[0].pos);
    //scene.worldEnv.lights[0].ambientIntensity = 0.0f; // 0.5f * (glm::sin(curTime * speed * 10.0f) + 1.0f);
    //scene.worldEnv.lights[0].specularIntensity = 0.5f * (glm::sin(curTime * speed * 10.0f) + 1.0f);

    //scene.worldEnv.lights[0].ambientIntensity = 0.5f * (glm::sin(curTime/1.0f) + 1.0f);
    //scene.worldEnv.lights[0].color.r = 0.5f * (glm::sin(curTime/2.0f) + 1.0f);
    //scene.worldEnv.lights[0].color.g = 1.0f - scene.worldEnv.lights[0].color.r;
    //scene.worldEnv.lights[0].color.b = 1.0f - scene.worldEnv.lights[0].color.r;
}


void MyApp::createLight() {
    glm::vec3 lightPos(5.f, 5.f, 10.f);
    glm::vec3 lightColor(1.f, 1.f, 1.f);

    scene.worldEnv.lights.push_back({ lightPos, lightColor, 1.0f, 1.0f });
    objects.lightBox = new rlgl::Object(assets.mesh.cube, assets.shader.colored, NO_MATERIAL);
    objects.lightBox->setPosition(lightPos);
    objects.lightBox->setColor(lightColor);
    objects.lightBox->setScale(0.4f);

    scene.addObject(objects.lightBox);
}

void MyApp::createBoxes() {

    objects.boxes = std::vector<rlgl::Object*>({
        new rlgl::Object(assets.mesh.cubeTex, assets.shader.texturedLightMat, assets.material.box),
        new rlgl::Object(assets.mesh.cubeTex, assets.shader.texturedLightMat, assets.material.boxJade),
        new rlgl::Object(assets.mesh.cubeTex, assets.shader.texturedLightMat, assets.material.boxGold),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.jade),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.gold),

        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.gold),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.gold),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.gold),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.gold),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.gold),
        
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.jade),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.jade),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.jade),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.jade),
        new rlgl::Object(assets.mesh.cube,    assets.shader.coloredLightMat,  assets.material.jade),

        new rlgl::Object(assets.mesh.cubeTex, assets.shader.texturedLightMat, assets.material.metalic),
        new rlgl::Object(assets.mesh.cubeTex, assets.shader.texturedLightMat, assets.material.metalic),
        new rlgl::Object(assets.mesh.cubeTex, assets.shader.texturedLightMat, assets.material.metalic),
        new rlgl::Object(assets.mesh.cubeTex, assets.shader.texturedLightMat, assets.material.metalic),
        new rlgl::Object(assets.mesh.cubeTex, assets.shader.texturedLightMat, assets.material.metalic)
        });


    objects.boxes[3]->setColor(glm::vec3(0.f, 0.659f, 0.42f)); //jade
    objects.boxes[4]->setColor(glm::vec3(1.f, 0.843f, 0.3f));  //gold
    
    objects.boxes[5]->setColor(glm::vec3(1.f, 0.f, 0.f));
    objects.boxes[6]->setColor(glm::vec3(0.f, 1.f, 0.f)); 
    objects.boxes[7]->setColor(glm::vec3(0.f, 0.f, 1.f)); 
    objects.boxes[8]->setColor(glm::vec3(0.9f, 0.9f, 0.1f)); 
    objects.boxes[9]->setColor(glm::vec3(0.9f, 0.1f, 0.9f)); 

    objects.boxes[10]->setColor(glm::vec3(1.f, 0.f, 0.f));
    objects.boxes[11]->setColor(glm::vec3(0.f, 1.f, 0.f));
    objects.boxes[12]->setColor(glm::vec3(0.f, 0.f, 1.f));
    objects.boxes[13]->setColor(glm::vec3(0.9f, 0.9f, 0.1f));
    objects.boxes[14]->setColor(glm::vec3(0.9f, 0.1f, 0.9f));

    int i = 0;
    float boxSize = BOX_WIDTH;
    glm::vec3 boxPos_0 = glm::vec3(10.f, boxSize, 4.f);
    glm::vec3 boxPos = boxPos_0;

	for (rlgl::Object* box : objects.boxes){
		box->setPosition(boxPos);
        box->setScale(boxSize);
        scene.addObject(box);

		boxPos.y -= boxSize * 1.5f; 
        if ((i + 1) % 5 == 0) {
            boxPos.z += boxSize * 1.5f;
            boxPos.y = boxPos_0.y;
        }
        i++;
	}


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

static int eventCount = 0;
bool MyApp::eventFilter(QObject* obj, QEvent *event){
    //std::cout << "eventCount: " << eventCount++ << std::endl;

    if(event->type() == QEvent::KeyPress){

        std::cout << "key" << std::endl;

        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_W) {
            std::cout <<"W" << std::endl;
            camera.moveForward(0.15f);
        }
        else if (ke->key() == Qt::Key_S) {
            std::cout <<"S" << std::endl;
            camera.moveBackward(0.15f);
        }

    }
    return QObject::eventFilter(obj, event);
}
bool MyApp::event(QEvent *event) {

    return QObject::event(event);
}

void MyApp::processInput(GLFWwindow* window) {
    //BaseApp::processInput(window);
    activeCamera->setFront(glm::vec3(1.0f, 0.0f, 0.0f));
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
