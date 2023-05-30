#include "Example8_App.h"
#include "rlMath.h"
#include "rlglMeshBank.h"
#include "rlglShaderBank.h"
#include "rlglDefaultMeshes.h"
#include <iostream>

const float MyApp::BOX_WIDTH = 2.5f;

MyApp::MyApp(const std::string& assetDirectory) : BaseApp(assetDirectory){}

void MyApp::prepareAssets() {

    std::string assetDirectory = rlgl::GlobalConfig::assetDirectory;

    cubeTexInstMesh = rlgl::MeshBank::defaultCube_tex();
    frustumMesh = rlgl::MeshBank::cube();

    glm::vec3 b1, b2, b3, b4, t1, t2, t3, t4;
    b1 = glm::vec3(10.f, 0.f, 0.f);
    b2 = b1 + glm::vec3(2.f, 0.f, 0.f);
    b3 = b2 + glm::vec3(0.f, 2.f, 0.f);
    b4 = b1 + glm::vec3(0.f, 2.f, 0.f);
    t1 = b1 + glm::vec3(0.f, 0.f, 2.f);
    t2 = b2 + glm::vec3(0.f, 0.f, 2.f);
    t3 = b3 + glm::vec3(0.f, 0.f, 2.f);
    t4 = b4 + glm::vec3(0.f, 0.f, 2.f);

    frustumMesh->vertices.data = rlgl::Mesh::createHexagonal(
        std::vector<glm::vec3>({ b1,b2,b3,b4 }),
        std::vector<glm::vec3>({ t1,t2,t3,t4 }));
    frustumMesh->initialize();
    
    frustumMesh->vertices.data = rlgl::Mesh::createHexagonal(
        std::vector<glm::vec3>({ b1,b2,b3,b4 }),
        std::vector<glm::vec3>({ t1,t2,t3,t4 + glm::vec3(0.f, 0.f, 1.f) }));
    frustumMesh->vertices.bindBuffer(GL_ARRAY_BUFFER);
    frustumMesh->vertices.bufferData(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    assets.mesh.world = rlgl::MeshBank::defaultPlane_textureX10();
    assets.mesh.cubeTex = rlgl::MeshBank::defaultCube_tex();
    assets.mesh.cube = rlgl::MeshBank::defaultCube();
    assets.mesh.frustum = frustumMesh;

    assets.material.checker = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/checker_grey.jpg", true);
    assets.material.box = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/box-texture.png", false);
    assets.material.boxMetal = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/metal-texture-1.jpg", false);

    assets.shader.textured = rlgl::ShaderBank::standardTextureShader();
    assets.shader.colored = rlgl::ShaderBank::standardColorShader();
}


int MyApp::prepareScene() {

    glClearColor(135.f/255.f, 206.f / 255.f, 250.f / 255.f, 1.0f);
    secondaryCam.aspectRatio = windowParams().aspect();
    secondaryCam.position = glm::vec3(70.f);
    secondaryCam.front = glm::vec3(-1.f);
    secondaryCam.upVector = glm::vec3(0.f, 0.f, 1.f);
    secondaryCam.aspectRatio = windowParams().aspect();
    secondaryCam.far = 800.f;

    prepareAssets();
    createWorld();
    createBoxes();
    //createFrustumObject();
    return 0;
}

void MyApp::createFrustumObject() {
    objects.frustum = new rlgl::Object(assets.mesh.frustum, assets.shader.colored, NO_MATERIAL);
    objects.frustum->setPosition(glm::vec3(0.f));
    objects.frustum->setScale(glm::vec3(1.f));
    objects.frustum->setColor(glm::vec4(0.f, 0.5f, 0.7f, 0.2f));
    scene.addObject(objects.frustum);
}

void MyApp::createWorld() {
    objects.worldPlane = new rlgl::Object(assets.mesh.world, assets.shader.textured, assets.material.checker);
    objects.worldPlane->setPosition(glm::vec3(0.f));
    objects.worldPlane->setScale(glm::vec3(100.f));
    scene.addObject(objects.worldPlane);
}

void MyApp::createBoxes() {
    rl::OctreeStruct octStruct({ 0.f, 0.f, 0.f }, 100.f, 4);
    octTree = rl::Octree(octStruct);

    int nBoxes = 1e3;
    glm::vec3 boxPos;
    rl::BoundingBox bbox;
    float boxSize = BOX_WIDTH/5.f;
    for (int i = 0; i < nBoxes; i++) {

        float z = rl::rand(0.f, 20.f);
        float rad = rl::rand(35.f, 40.f);
        float alpha = rl::rand(0.f, glm::pi<float>() * 2.f);
        boxPos = glm::vec3(rad * glm::cos(alpha), rad * glm::sin(alpha), z);
        //boxPos = glm::vec3(rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f), rl::rand(BOX_WIDTH / 2.f, 10.f*BOX_WIDTH));
        //alpha = 2.f * glm::pi<float>() * (float)i / (float)nBoxes;
        //boxPos = glm::vec3(35.f * glm::cos(alpha), 35.f * glm::sin(alpha), 10.f);
        bbox = rl::BoundingBox::createCubeBoundingBox(boxPos, boxSize);

        objects.cubes.push_back(new rlgl::Object(assets.mesh.cubeTex, assets.shader.textured, assets.material.box));
        objects.cubes[i]->setPosition(boxPos);
        objects.cubes[i]->setScale(glm::vec3(boxSize));
        objects.cubes[i]->setColor(glm::vec4(0.f, 1.0f, 0.5f, 1.f));
        rl::OctreeItem* item = octTree.addObject((void*)objects.cubes[i], bbox);
        scene.addObject(objects.cubes[i]);
    }
    std::cout << octTree.toStr() << "\n";
    octTree.callOnAllOctTreeObjectWithAddress(OctTreeFunc::setHighlightColorBlue, "1", true);
    octTree.callOnAllOctTreeObjectWithAddress(OctTreeFunc::setHighlightColorBlue, "2", true);
    octTree.callOnAllOctTreeObjectWithAddress(OctTreeFunc::setHighlightColorRed,  "3", true);
    octTree.callOnAllOctTreeObjectWithAddress(OctTreeFunc::setHighlightColorGreen,"4", true);
    //octTree.callOnAllOctTreeObjectWithAddress(OctTreeFunc::setHighlightColorBlue, "5", true);
    //octTree.callOnAllOctTreeObjectWithAddress(OctTreeFunc::setHighlightColorBlue, "6", true);
    //octTree.callOnAllOctTreeObjectWithAddress(OctTreeFunc::setHighlightColorRed,  "7", true);
    //octTree.callOnAllOctTreeObjectWithAddress(OctTreeFunc::setHighlightColorGreen,"8", true);
}

int MyApp::updateScene() {
    camera.position.z = 2.f;
    camera.computeFrustum();
    std::vector<glm::vec3> near, far;
    camera.frustumCorners(near, far);
    frustumMesh->vertices.data = rlgl::Mesh::createHexagonal(near, far);
    frustumMesh->vertices.bindBuffer(GL_ARRAY_BUFFER);
    frustumMesh->vertices.bufferData(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    if(octreeFrustumCullingON){
        octTree.callOnOctTreeObjects(
            &OctTreeFunc::isInFrustum,
            &OctTreeFunc::hide, 
            &camera.frustum);
    }

    updateBoxes();

    return 0;
}

int MyApp::postRender(){
    octTree.callOnAllOctTreeObject(OctTreeFunc::unhide);
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
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        octreeFrustumCullingON = !octreeFrustumCullingON;
        std::cout << "Octree frustum culling in " << std::string(octreeFrustumCullingON ? "ON" : "OFF") << std::endl;
    }

    BaseApp::processInput(window);
}



void MyApp::updateBoxes() {

    double curTime = glfwGetTime();
    for (rlgl::Object* obj : objects.cubes) {

        obj->translate(glm::vec3(0.1f * glm::sin(curTime * 2.5f), 0.f, 0.f));
        rl::BoundingBox bbox = rl::BoundingBox::createCubeBoundingBox(obj->getPosition(), BOX_WIDTH);

        octTree.moveObject(obj, bbox);
    }

}

bool OctTreeFunc::isInFrustum(const rl::BoundingBox& boundingBox, void* frustumPtr){
    const rlgl::Frustum& frustum = *(rlgl::Frustum*)frustumPtr;
    return rlgl::isInFrustum(frustum, boundingBox);
}


/*!*/
bool OctTreeFunc::isInFrustum(rl::OctreeItem* octreeNode, void* frustumPtr) {
    const rlgl::Frustum& frustum = *(rlgl::Frustum*)frustumPtr;
    return rlgl::isInFrustum(frustum, octreeNode->boundingBox);
}

/*!*/
void OctTreeFunc::hideIfOutsideFrustum(void* object, const rl::BoundingBox& bbox, void* frustumPtr) {
    const rlgl::Frustum& frustum = *(rlgl::Frustum*)frustumPtr;
    if (!rlgl::isInFrustum(frustum, bbox)) {
        ((rlgl::Object*)object)->setInViewState(false);
    }
}

/*!*/
void OctTreeFunc::unhide(void* object, const rl::BoundingBox& bbox, void*) {
    ((rlgl::Object*)object)->setInViewState(true);
}

/*!*/
void OctTreeFunc::hide(void* object) {
    ((rlgl::Object*)object)->setInViewState(false);
}

void OctTreeFunc::setHighlight(void* object) {
	((rlgl::Object*)object)->setHighlight(true);
}
void OctTreeFunc::setNoHighlight(void* object) {
	((rlgl::Object*)object)->setHighlight(false);
}

void OctTreeFunc::setHighlightColorRed(void* object) {
    OctTreeFunc::setHighlight(object);
    ((rlgl::Object*)object)->setColor(glm::vec3(1.f, 0.f, 0.f));
}
void OctTreeFunc::setHighlightColorGreen(void* object) {
    OctTreeFunc::setHighlight(object);
    ((rlgl::Object*)object)->setColor(glm::vec3(0.f, 1.f, 0.f));
}
void OctTreeFunc::setHighlightColorBlue(void* object) {
    OctTreeFunc::setHighlight(object);
    ((rlgl::Object*)object)->setColor(glm::vec3(0.f, 0.f, 1.f));
}

