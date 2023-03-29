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
    //http://devernay.free.fr/cours/opengl/materials.html
    rlgl::TexturedMaterial* materialChecker = new rlgl::TextureLightPropMaterial();
    materialChecker->defineTexture(_assetDirectory + "\\textures\\checker_grey.jpg", true);
    assetIDs.material.checker = scene.addMaterial(materialChecker);
    
    rlgl::TextureLightPropMaterial* materialBox1 = new rlgl::TextureLightPropMaterial();
    materialBox1->defineTexture(_assetDirectory + "\\textures\\box-texture.png", false);
    materialBox1->setProperties(
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        128.0f
    );
    assetIDs.material.box = scene.addMaterial(materialBox1);

    rlgl::TextureLightPropMaterial* materialBox2 = new rlgl::TextureLightPropMaterial();
    materialBox2->defineTexture(_assetDirectory + "\\textures\\box-texture.png", false);
    materialBox2->setProperties(/*jade*/
        glm::vec3(0.135, 0.2225, 0.1575),
        glm::vec3(0.54, 0.89, 0.63),
        glm::vec3(0.316228, 0.316228, 0.316228),
        128.0f * 0.1f
    );
    assetIDs.material.boxJade = scene.addMaterial(materialBox2);

    rlgl::TextureLightPropMaterial* materialBox3 = new rlgl::TextureLightPropMaterial();
    materialBox3->defineTexture(_assetDirectory + "\\textures\\box-texture.png", false);
    materialBox3->setProperties(/*gold*/
        glm::vec3(0.24725, 0.1995, 0.0745),
        glm::vec3(0.75164, 0.60648, 0.22648),
        glm::vec3(0.628281, 0.555802, 0.366065),
        128.0f * 0.4f
    );
    assetIDs.material.boxGold = scene.addMaterial(materialBox3);

    rlgl::LightPropMaterial* materialBox4 = new rlgl::LightPropMaterial();
    materialBox2->setProperties(/*jade*/
        glm::vec3(0.135, 0.2225, 0.1575),
        glm::vec3(0.54, 0.89, 0.63),
        glm::vec3(0.316228, 0.316228, 0.316228),
        128.0f * 0.1f
    );
    assetIDs.material.jade = scene.addMaterial(materialBox4);

    rlgl::LightPropMaterial* materialBox5 = new rlgl::LightPropMaterial();
    materialBox5->setProperties(/*gold*/
        glm::vec3(0.24725, 0.1995, 0.0745),
        glm::vec3(0.75164, 0.60648, 0.22648),
        glm::vec3(0.628281, 0.555802, 0.366065),
        128.0f * 0.4f
    );    
    assetIDs.material.gold = scene.addMaterial(materialBox5);

    rlgl::TextureLightPropMaterial* materialBox6 = new rlgl::TextureLightPropMaterial();
    materialBox6->defineTexture(_assetDirectory + "\\textures\\metal-texture-1.jpg", false);
    materialBox6->setProperties(/*silver*/
        glm::vec3(0.19225, 0.19225, 0.19225),
        glm::vec3(0.50754, 0.50754, 0.50754),
        glm::vec3(0.508273, 0.508273, 0.508273),
        128.0f * 0.5f
    );
    assetIDs.material.metalic = scene.addMaterial(materialBox6);

    //Shaders:
    rlgl::TextureShader* shaderTextured = new rlgl::TextureShader(
        _assetDirectory + "\\shaders\\object_tex.vs", 
        _assetDirectory + "\\shaders\\object_tex.fs"
    );
    assetIDs.shader.textured = scene.addShader(shaderTextured);

    rlgl::TextureLightShader* shaderTexturedLight = new rlgl::TextureLightShader(
        _assetDirectory + "\\shaders\\object_tex_light.vs", 
        _assetDirectory + "\\shaders\\object_tex_light.fs");
    assetIDs.shader.texturedLight = scene.addShader(shaderTexturedLight);

    rlgl::TextureLightMaterialShader* shaderTexturedLightMat = new rlgl::TextureLightMaterialShader(
        _assetDirectory + "\\shaders\\object_tex_light_mat.vs", 
        _assetDirectory + "\\shaders\\object_tex_light_mat.fs");
    assetIDs.shader.texturedLightMat = scene.addShader(shaderTexturedLightMat);

    rlgl::LightMaterialShader* shaderLightMat = new rlgl::LightMaterialShader(
        _assetDirectory + "\\shaders\\object_col_light_mat.vs",
        _assetDirectory + "\\shaders\\object_col_light_mat.fs");
    assetIDs.shader.coloredLightMat = scene.addShader(shaderLightMat);

    rlgl::StandardShader* shaderColored = new rlgl::StandardShader(
        _assetDirectory + "\\shaders\\object_col.vs", 
        _assetDirectory + "\\shaders\\object_col.fs");
    assetIDs.shader.colored = scene.addShader(shaderColored);


    //UI:
    rlgl::primitive_mesh::square.initialize();
    assetIDs.mesh.square = uiScene.addMesh(&rlgl::primitive_mesh::square);

    rlgl::StandardShader* uiShader = new rlgl::StandardShader(
        _assetDirectory + "\\shaders\\ui_element.vs", 
        _assetDirectory + "\\shaders\\ui_element.fs");

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
    objects.worldPlane = new rlgl::Object(
        assetIDs.mesh.world, 
        assetIDs.shader.texturedLight, 
        assetIDs.material.checker);

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

    double speed = 0.2f;
    double rad = 15.0f;
    scene.worldEnv.lights[0].pos =
        glm::vec3(4.0f, -5.0f, 15.0f) + glm::vec3(0.f * glm::sin(speed * curTime), rad * glm::cos(speed * curTime), 0.f);


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
    objects.lightBox = new rlgl::Object(assetIDs.mesh.cube, assetIDs.shader.colored, NO_MATERIAL);
    objects.lightBox->setPosition(lightPos);
    objects.lightBox->setColor(lightColor);
    objects.lightBox->setScale(0.4f);

    scene.addObject(objects.lightBox);
}

void MyApp::createBoxes() {

    objects.boxes = std::vector<rlgl::Object*>({
        new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.texturedLightMat, assetIDs.material.box),
        new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.texturedLightMat, assetIDs.material.boxJade),
        new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.texturedLightMat, assetIDs.material.boxGold),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.jade),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.gold),

        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.gold),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.gold),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.gold),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.gold),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.gold),
        
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.jade),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.jade),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.jade),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.jade),
        new rlgl::Object(assetIDs.mesh.cube,    assetIDs.shader.coloredLightMat,  assetIDs.material.jade),

        new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.texturedLightMat, assetIDs.material.metalic),
        new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.texturedLightMat, assetIDs.material.metalic),
        new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.texturedLightMat, assetIDs.material.metalic),
        new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.texturedLightMat, assetIDs.material.metalic),
        new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.texturedLightMat, assetIDs.material.metalic)
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
        objects.axes.push_back(new rlgl::Object(assetIDs.mesh.cube, assetIDs.shader.colored, UINT32_MAX));
        objects.axes[i]->setColor(axesColor[i]);
        objects.axes[i]->setPosition(axesDir[i] * axesL / 2.f);
        objects.axes[i]->setScale(axesScales[i]);
        scene.addObject(objects.axes[i]);
    }
}


static rlgl::Object* lastHitObj = nullptr;
int MyApp::updateScene() {
    //camera.position.z = 2.f;
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