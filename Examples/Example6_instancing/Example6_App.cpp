#include "Example5_App.h"
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
    assets.mesh.terrainDummy = rlgl::MeshBank::defaultPlane();
    assets.mesh.grass = rlgl::MeshBank::defaultGrass();
    assets.mesh.frustum = frustumMesh;

    assets.material.checker = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/checker_grey.jpg", true);
    assets.material.box = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/box-texture.png", false);
    assets.material.boxMetal = std::make_shared<rlgl::Textured2dMaterial>(
        assetDirectory + "/textures/metal-texture-1.jpg", false);

    rlgl::ShaderPtr shaderInst = std::make_shared<rlgl::Shader>(assetDirectory + "\\shaders\\object_inst.vs", assetDirectory + "\\shaders\\object_inst.fs");
    rlgl::ShaderPtr shaderGrass = std::make_shared<rlgl::Shader>(assetDirectory + "\\shaders\\grass.vs", assetDirectory + "\\shaders\\grass.fs");
    
    assets.shader.textured = rlgl::ShaderBank::standardTextureShader();
    assets.shader.colored = rlgl::ShaderBank::standardColorShader();
    assets.shader.inst = shaderInst;
    assets.shader.grass = shaderGrass;

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

    prepareAssets();
    createWorld();
    createLODterrain();
    createBoxes();
    createCSYS();
    createUI();
    //createFrustumObject();
	createGrass();
    return 0;
}

void MyApp::createFrustumObject() {
    objects.frustum = new rlgl::Object(assets.mesh.frustum, assets.shader.colored, NO_MATERIAL);
    objects.frustum->setPosition(glm::vec3(0.f));
    objects.frustum->setScale(glm::vec3(1.f));
    objects.frustum->setColor(glm::vec4(0.f, 0.5f, 0.7f, 0.2f));
    scene.addObject(objects.frustum);
}

void MyApp::createGrass() {

    glm::vec3 gpos(4., 4., 0.);

    for(int i = 0; i < 1; i++){
	    rlgl::Object* grassObj = new rlgl::Object(assets.mesh.grass, assets.shader.grass, NO_MATERIAL);
	    grassObj->setPosition(glm::vec3(
            gpos.x + rl::rand(0.0, 2.0), 
            gpos.y + rl::rand(0.0, 2.0), 
            gpos.z));
	    grassObj->setScale(glm::vec3(2.f));
	    grassObj->setColor(glm::vec4(0.1f, 0.7f, 0.1f, 1.0f));

	    objects.grass.push_back(grassObj);
	    scene.addObject(grassObj);
    }
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


    int nInstances = 1e4;
    for (int i = 0; i < nInstances; i++) {
        boxPos = glm::vec3(rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f));

        float rad = rl::rand(40.f, 50.f);
        float alpha = rl::rand(0.f, glm::pi<float>() * 2.f);
		float x = rl::rand(0.f, 1.f);
		float theta = glm::acos(2.f*x - 1.f);
        //float theta = rl::rand(0.f, glm::pi<float>() * 2.f);
        boxPos = glm::vec3(rad * glm::sin(alpha) * glm::cos(theta), rad * glm::sin(alpha) * glm::sin(theta), rad * glm::cos(alpha));

        glm::mat4 tMat = glm::translate(glm::mat4(1.f), boxPos);
        glm::mat4 sMat = glm::scale(glm::mat4(1.f), glm::vec3(0.25f));
        glm::mat4 mMat = tMat * sMat;
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                cubeTexInstMesh->instances.data.push_back(mMat[j][k]);
            }
        }
    }
    cubeTexInstMesh->hasInstances = true;
    cubeTexInstMesh->initialize();
    assets.mesh.cubeInst = cubeTexInstMesh;

    objects.instObj = new rlgl::Object(assets.mesh.cubeInst, assets.shader.inst, assets.material.boxMetal);
    objects.instObj->setNinstances(nInstances);
    scene.addObject(objects.instObj);

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


void MyApp::createCSYS() {
    float axesL = 5.f;
    float axesW = 0.2f;
    std::vector<glm::vec3> axesDir({ glm::vec3(1.f, 0.f, 0.f),glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f , 0.f, 1.f) });
    std::vector<glm::vec3> axesScales({ glm::vec3(axesL, axesW, axesW),glm::vec3(axesW, axesL, axesW), glm::vec3(axesW, axesW, axesL) });
    std::vector<glm::vec4> axesColor({ glm::vec4(1.f, 0.f, 0.f, 1.f),glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec4(0.f, 0.f, 1.f, 1.f) });
    for (int i = 0; i < axesDir.size(); i++) {
        objects.axes.push_back(new rlgl::Object(assets.mesh.cube, assets.shader.colored));
        objects.axes[i]->setColor(axesColor[i]);
        objects.axes[i]->setPosition(axesDir[i] * axesL / 2.f);
        objects.axes[i]->setScale(axesScales[i]);
        scene.addObject(objects.axes[i]);
    }
}


static rlgl::Object* lastHitObj = nullptr;
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

    updateHitTestOctTree();
    updateBoxes();
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

