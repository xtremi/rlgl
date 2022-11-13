#include "MyApp.h"
#include "rlMath.h"
#include <iostream>

const float MyApp::BOX_WIDTH = 2.f;

void MyApp::prepareAssets() {

    cubeTexInstMesh = rlgl::primitive_mesh::cube_tex;


    rlgl::primitive_mesh::plane_textureX10.initialize();
    rlgl::primitive_mesh::plane.initialize();
    rlgl::primitive_mesh::cube_tex.initialize();
    rlgl::primitive_mesh::cube.initialize();
    rlgl::primitive_mesh::terrainDummy.initialize();

    assetIDs.mesh.world = scene.addMesh(&rlgl::primitive_mesh::plane_textureX10);
    assetIDs.mesh.cubeTex = scene.addMesh(&rlgl::primitive_mesh::cube_tex);
    assetIDs.mesh.cube = scene.addMesh(&rlgl::primitive_mesh::cube);
    assetIDs.mesh.terrainDummy = scene.addMesh(&rlgl::primitive_mesh::terrainDummy);


    rlgl::Material materialChecker, materialBox, materialBoxMetal;
    materialChecker.initialize("..\\data\\textures\\checker_grey.jpg", true);
    materialBox.initialize("..\\data\\textures\\box-texture.png", false);
    materialBoxMetal.initialize("..\\data\\textures\\box_metal.jpg", false);

    assetIDs.material.checker = scene.addMaterial(materialChecker);
    assetIDs.material.box = scene.addMaterial(materialBox);
    assetIDs.material.boxMetal = scene.addMaterial(materialBoxMetal);

    rlgl::Shader shaderTextured, shaderColored, shaderInst;
    shaderTextured.initialize("..\\data\\shaders\\object.vs", "..\\data\\shaders\\object.fs");
    shaderTextured.setInt("textureID", materialChecker.glID);
    shaderColored.initialize("..\\data\\shaders\\object_col.vs", "..\\data\\shaders\\object_col.fs"); 
    shaderInst.initialize("..\\data\\shaders\\object_inst.vs", "..\\data\\shaders\\object_inst.fs");

    assetIDs.shader.textured = scene.addShader(shaderTextured);
    assetIDs.shader.colored = scene.addShader(shaderColored);
    assetIDs.shader.inst = scene.addShader(shaderInst);


    //UI:
    rlgl::primitive_mesh::square.initialize();
    assetIDs.mesh.square = uiScene.addMesh(&rlgl::primitive_mesh::square);

    rlgl::Shader uiShader;
    uiShader.initialize("..\\data\\shaders\\ui_element.vs", "..\\data\\shaders\\ui_element.fs");

    assetIDs.shader.ui = uiScene.addShader(uiShader);
}


int MyApp::prepareScene() {

    lodControl = rlgl::LODcontroller(glm::vec3(0.f), 15.f, 4, 0.5);

    secondaryCam.aspectRatio = windowParams().aspect();

    secondaryCam.position = glm::vec3(70.f);
    secondaryCam.front = glm::vec3(-1.f);
    secondaryCam.upVector = glm::vec3(0.f, 0.f, 1.f);
    secondaryCam.aspectRatio = windowParams().aspect();

    prepareAssets();
    createWorld();
    createLODterrain();
    //createBoxes();
    createCSYS();
    createUI();
    return 0;
}

void MyApp::createWorld() {
    objects.worldPlane = new rlgl::Object(assetIDs.mesh.world, assetIDs.shader.textured, assetIDs.material.checker);
    objects.worldPlane->setPosition(glm::vec3(0.f));
    objects.worldPlane->setScale(glm::vec3(100.f));
    scene.addObject(objects.worldPlane);
}

void MyApp::createUI() {
    uiObjects.aimCross.push_back(new rlgl::Object(assetIDs.mesh.square, assetIDs.shader.ui, INT64_MAX));
    uiObjects.aimCross.push_back(new rlgl::Object(assetIDs.mesh.square, assetIDs.shader.ui, INT64_MAX));
    uiObjects.aimCross[0]->setPosition(glm::vec3(0.f, 0.f, 0.2f));
    uiObjects.aimCross[0]->setScale(glm::vec3(0.2f, 0.01f, 1.f));
    uiObjects.aimCross[1]->setPosition(glm::vec3(0.0f, 0.0f, 0.2f));
    uiObjects.aimCross[1]->setScale(glm::vec3(0.01f, 0.2f, 1.f));
    uiObjects.aimCross[0]->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
    uiObjects.aimCross[1]->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
    uiScene.addObject(uiObjects.aimCross[0]);
    uiScene.addObject(uiObjects.aimCross[1]);
}

void MyApp::createBoxes() {
    rl::OctreeStruct octStruct({ 0.f, 0.f, 0.f }, 350.f, 5);
    octTree = rl::Octree(octStruct);

    int nBoxes = 100;
    glm::vec3 boxPos;
    rl::BoundingBox bbox;
    float boxSize = BOX_WIDTH;
    for (int i = 0; i < nBoxes; i++) {

        boxPos = glm::vec3(rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f));

        bbox = rl::BoundingBox::createCubeBoundingBox(boxPos, boxSize);

        objects.cubes.push_back(new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.textured, assetIDs.material.box));
        objects.cubes[i]->setPosition(boxPos);
        objects.cubes[i]->setScale(glm::vec3(boxSize));
        rl::OctreeItem* item = octTree.addObject((void*)objects.cubes[i], bbox);
        scene.addObject(objects.cubes[i]);
    }

    int nInstances = 1e4;
    for (int i = 0; i < nInstances; i++) {
        boxPos = glm::vec3(rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f));
        glm::mat4 tMat = glm::translate(glm::mat4(1.f), boxPos);
        glm::mat4 sMat = glm::scale(glm::mat4(1.f), glm::vec3(0.25f));
        glm::mat4 mMat = tMat * sMat;
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                cubeTexInstMesh.instances.data.push_back(mMat[j][k]);
            }
        }
    }
    cubeTexInstMesh.hasInstances = true;
    cubeTexInstMesh.initialize();
    assetIDs.mesh.cubeInst = scene.addMesh(&cubeTexInstMesh);

    objects.instObj = new rlgl::Object(assetIDs.mesh.cubeInst, assetIDs.shader.inst, assetIDs.material.boxMetal);
    objects.instObj->setNinstances(nInstances);
    scene.addObject(objects.instObj);

}

void MyApp::createLODterrain() {
    
    float zpos = 1.f;

    TerrainQuadObject* terrainQuad_0 = new TerrainQuadObject(assetIDs.mesh.terrainDummy, assetIDs.shader.colored, 0, rlgl::LODloc::center);
    objects.terrainLODquads.push_back(terrainQuad_0);

    glm::vec3 quadPos = glm::vec3(lodControl.quadPosition(rlgl::LODloc::center, 0), zpos);
    terrainQuad_0->setPosition(quadPos);

    float quadSize = lodControl.quadSideLength(0);
    terrainQuad_0->setScale(glm::vec3(quadSize, quadSize, 1.f));
    terrainQuad_0->setColor(glm::vec4(1.f, 0.5f, 0.5f, 1.f));
    scene.addObject(terrainQuad_0);

    rl::BoundingBox bbox = rl::BoundingBox::createCubeBoundingBox(glm::vec3(quadPos), glm::vec3(quadSize, quadSize, 1.f));

    std::vector<glm::vec3> colors({
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(1.f, 0.f, 1.f),
        glm::vec3(1.f, 1.f, 0.f) });
    for (int i = 0; i < lodControl.levels(); i++) { //level

        for (int j = 0; j < 8; j++) { //location

            rlgl::LODloc loc = (rlgl::LODloc)j;

            TerrainQuadObject* terrainQuad = new TerrainQuadObject(assetIDs.mesh.terrainDummy, assetIDs.shader.colored, i, loc);
            objects.terrainLODquads.push_back(terrainQuad);

            quadPos = glm::vec3(lodControl.quadPosition(loc, i), zpos);
            terrainQuad->setPosition(quadPos);
            
            quadSize = lodControl.quadSideLength(i);
            terrainQuad->setScale(glm::vec3(quadSize, quadSize, 1.f));
            terrainQuad->setColor(colors[i] * 0.1f * (float)(j + 1));
            scene.addObject(terrainQuad);
        }
       
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

    camera.computeFrustum();
    octTree.callOnAllOctTreeObject(&OctTreeFunc::hideIfInFrustum, &camera.frustum);


    updateHitTestOctTree();
    updateCubes();
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
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        activeCamera = &camera;
    }

    BaseApp::processInput(window);
}


void MyApp::updateTerrainLOD() {

    for (rlgl::Object* obj : objects.terrainLODquads) {
        rl::BoundingBox bbox = rl::BoundingBox::createCubeBoundingBox(obj->getPosition(), obj->getScale());
        if (!rlgl::isInFrustum(camera.frustum, bbox)) {
            (obj->setInViewState(false));
        }
    }


    if (lodControl.outOfCenterLimit(camera.position)) {

        lodControl.setCenterPosition(camera.position);

        for (TerrainQuadObject* tobj : objects.terrainLODquads) {
            tobj->setPosition(glm::vec3(lodControl.quadPosition(tobj->loc, tobj->level), 1.f));

        }


    }

}


/*!
    Performs hittest on objects in OctTree
    - spins and highligh object if hit
*/
void MyApp::updateHitTestOctTree() {
    rl::Ray hitRay(camera.lookVec(), camera.position);
    rlgl::Object* hitObj = nullptr;
    if (octTree.hitTest(hitRay, (void**)&hitObj)) {
        if (lastHitObj && (lastHitObj != hitObj)) lastHitObj->setHighlight(false);

        hitObj->rotate(0.01f, glm::vec3(0.f, 0.f, 1.f));
        hitObj->setHighlight(true);
        lastHitObj = hitObj;
    }
    else {
        if (lastHitObj) lastHitObj->setHighlight(false);
    }
}


void MyApp::updateCubes() {

    double curTime = glfwGetTime();
    for (rlgl::Object* obj : objects.cubes) {

        obj->translate(glm::vec3(0.1f * glm::sin(curTime * 2.5f), 0.f, 0.f));
        rl::BoundingBox bbox = rl::BoundingBox::createCubeBoundingBox(obj->getPosition(), BOX_WIDTH);

        octTree.moveObject(obj, bbox);
    }



}


void OctTreeFunc::hideIfInFrustum(void* object, const rl::BoundingBox& bbox, void* frustumPtr) {
    const rlgl::Frustum& frustum = *(rlgl::Frustum*)frustumPtr;
    if (!rlgl::isInFrustum(frustum, bbox)) {
        ((rlgl::Object*)object)->setInViewState(false);
    }
}

bool OctTreeFunc::isInFrustumOrHide(void* object, const rl::BoundingBox& bbox, void* frustumPtr) {
    const rlgl::Frustum& frustum = *(rlgl::Frustum*)frustumPtr;
    if (!rlgl::isInFrustum(frustum, bbox)) {
        ((rlgl::Object*)object)->setInViewState(false);
        return false;
    }
    return true;
}


void OctTreeFunc::unhide(void* object, const rl::BoundingBox& bbox, void*) {
    ((rlgl::Object*)object)->setInViewState(true);
}

