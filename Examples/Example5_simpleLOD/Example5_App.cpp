#include "Example5_App.h"
#include "rlMath.h"
#include "rlglMeshBank.h"
#include "rlglShaderBank.h"
#include "rlglColors.h"
#include "rlglBaseModel.h"
#include <iostream>

const float MyApp::BOX_WIDTH = 2.5f;

MyApp::MyApp(const std::string& assetDirectory) : BaseApp(assetDirectory){}

void MyApp::prepareAssets() {

    std::string assetDirectory = rlgl::GlobalConfig::assetDirectory;
    assets.mesh.cube = rlgl::MeshBank::defaultCube();
    assets.mesh.terrainDummy = rlgl::MeshBank::defaultPlane();

    assets.shader.textured = rlgl::ShaderBank::standardTextureShader();
    assets.shader.colored = rlgl::ShaderBank::standardColorShader();

    //UI:
    assets.mesh.square = rlgl::MeshBank::defaultSquare();
    assets.shader.ui = rlgl::ShaderBank::standardUIcolShader();
}


int MyApp::prepareScene() {

    glClearColor(135.f/255.f, 206.f / 255.f, 250.f / 255.f, 1.0f);

    lodControl = rlgl::LODcontroller(glm::vec3(0.f), 10.f, 4, 0.5);

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
    createLODterrain();
    return 0;
}

void MyApp::createLODterrain() {
    
    float zpos = 0.f;

    TerrainQuadObject* terrainQuad_0 = new TerrainQuadObject(
        assets.mesh.terrainDummy, assets.shader.colored, 0, rlgl::LODloc::center);
    objects.terrainLODquads.push_back(terrainQuad_0);

    glm::vec3 quadPos = glm::vec3(lodControl.quadPosition(rlgl::LODloc::center, 0), zpos);
    terrainQuad_0->setPosition(quadPos);

    float quadSize = lodControl.quadSideLength(0);
    terrainQuad_0->setScale(glm::vec3(quadSize, quadSize, 1.f));
    terrainQuad_0->setColor(glm::vec4(1.f, 0.5f, 0.5f, 1.f));
    scene.addObject(terrainQuad_0);

    //rl::BoundingBox bbox = rl::BoundingBox::createCubeBoundingBox(glm::vec3(quadPos), glm::vec3(quadSize, quadSize, 1.f));
	//octTree.addObject(terrainQuad_0, bbox);

    std::vector<glm::vec3> colors({
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(1.f, 0.f, 1.f),
        glm::vec3(1.f, 1.f, 0.f) });
    for (int i = 0; i < lodControl.levels(); i++) { //level

        for (int j = 0; j < 8; j++) { //location

            rlgl::LODloc loc = (rlgl::LODloc)j;

            TerrainQuadObject* terrainQuad = new TerrainQuadObject(assets.mesh.terrainDummy, assets.shader.colored, i, loc);
            objects.terrainLODquads.push_back(terrainQuad);

            quadPos = glm::vec3(lodControl.quadPosition(loc, i), zpos);
            terrainQuad->setPosition(quadPos);
            
            quadSize = lodControl.quadSideLength(i);
            terrainQuad->setScale(glm::vec3(quadSize, quadSize, 1.f));
            terrainQuad->setColor(colors[i] * (0.5f + 0.5f*(float)j/8));
            scene.addObject(terrainQuad);

			//bbox = rl::BoundingBox::createCubeBoundingBox(glm::vec3(quadPos), glm::vec3(quadSize, quadSize, 1.f));
			//octTree.addObject(terrainQuad, bbox);
        }
       
    }

}

static rlgl::Object* lastHitObj = nullptr;
int MyApp::updateScene() {
    camera.position.z = 2.f;
    camera.computeFrustum();
    std::vector<glm::vec3> near, far;
    camera.frustumCorners(near, far);

    if(octreeFrustumCullingON){
        octTree.callOnOctTreeObjects(
            &OctTreeFunc::isInFrustum,
            &OctTreeFunc::hide, 
            &camera.frustum);
    }

    updateTerrainLOD();
    return 0;
}

int MyApp::postRender(){

    octTree.callOnAllOctTreeObject(OctTreeFunc::unhide);
    for (rlgl::Object* obj : objects.terrainLODquads) {
        obj->setInViewState(true);
    }


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


void MyApp::updateTerrainLOD() {

    for (int i = 1; i < objects.terrainLODquads.size(); i++) {
        rl::BoundingBox bbox = rl::BoundingBox::createCubeBoundingBox(
            objects.terrainLODquads[i]->getPosition(), objects.terrainLODquads[i]->getScale());
        if (!rlgl::isInFrustum(camera.frustum, bbox, true)) {
            (objects.terrainLODquads[i]->setInViewState(false));
        }
    }

    if (lodControl.outOfCenterLimit(camera.position)) {
		glm::vec3 newPos(camera.position.x, camera.position.y, 0.f);
		lodControl.setCenterPosition(newPos);
		
        for (TerrainQuadObject* tobj : objects.terrainLODquads) {
            tobj->setPosition(glm::vec3(lodControl.quadPosition(tobj->loc, tobj->level), 0.f));
        }
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

