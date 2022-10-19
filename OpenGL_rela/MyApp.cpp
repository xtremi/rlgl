#include "MyApp.h"
#include "rlMath.h"
#include <iostream>

void MyApp::processInput(GLFWwindow* window) {
    BaseApp::processInput(window);
}

void MyApp::prepareAssets() {

    rlgl::primitive_mesh::plane_textureX10.initialize();
    rlgl::primitive_mesh::plane.initialize();
    rlgl::primitive_mesh::cube_tex.initialize();
    rlgl::primitive_mesh::cube.initialize();
    rlgl::primitive_mesh::square.initialize();

    assetIDs.mesh.world = scene.addMesh(&rlgl::primitive_mesh::plane_textureX10);
    assetIDs.mesh.cubeTex = scene.addMesh(&rlgl::primitive_mesh::cube_tex);
    assetIDs.mesh.cube = scene.addMesh(&rlgl::primitive_mesh::cube);
    assetIDs.mesh.square = uiScene.addMesh(&rlgl::primitive_mesh::square);

    rlgl::Material materialChecker, materialBox;
    materialChecker.initialize("..\\data\\textures\\checker_grey.jpg", true);
    materialBox.initialize("..\\data\\textures\\box-texture.png", false);

    assetIDs.material.checker = scene.addMaterial(materialChecker);
    assetIDs.material.box = scene.addMaterial(materialBox);


    rlgl::Shader shaderTextured, shaderColored, uiShader;
    shaderTextured.initialize("..\\data\\shaders\\object.vs", "..\\data\\shaders\\object.fs");
    shaderTextured.setInt("textureID", materialChecker.glID);
    shaderColored.initialize("..\\data\\shaders\\object_col.vs", "..\\data\\shaders\\object_col.fs"); 
    uiShader.initialize("..\\data\\shaders\\ui_element.vs", "..\\data\\shaders\\ui_element.fs");

    assetIDs.shader.textured = scene.addShader(shaderTextured);
    assetIDs.shader.colored = scene.addShader(shaderColored);
    assetIDs.shader.ui = uiScene.addShader(uiShader);
}


int MyApp::prepareScene() {
    prepareAssets();
    
    createWorld();
    createBoxes();
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
    rl::OctreeStruct octStruct({ 0.f, 0.f, 0.f }, 100.f, 5);
    octTree = rl::Octree(octStruct);

    int nBoxes = 500;
    glm::vec3 boxPos;
    rl::BoundingBox bbox;
    float boxSize = 2.f;
    for (int i = 0; i < nBoxes; i++) {

        boxPos = glm::vec3(rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f));
        bbox = rl::BoundingBox::createCubeBoundingBox(boxPos, boxSize);

        objects.cubes.push_back(new rlgl::Object(assetIDs.mesh.cubeTex, assetIDs.shader.textured, assetIDs.material.box));
        objects.cubes[i]->setPosition(boxPos);
        objects.cubes[i]->setScale(glm::vec3(boxSize));
        rl::OctreeItem* item = octTree.addObject((void*)objects.cubes[i], bbox);
        scene.addObject(objects.cubes[i]);
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

    double curTime = glfwGetTime();

	/************************/
	camera.computeFrustum();
	auto it = octTree.octStructTreeItemMap.begin();
	for (it; it != octTree.octStructTreeItemMap.end(); it++) {

		auto it2 = it->second->objects.begin();
		for (it2; it2 != it->second->objects.end(); it2++) {

			if (!rlgl::isInFrustum(camera.frustum, it2->bbox)) {
				((rlgl::Object*)it2->data)->setInViewState(false);
			}

		}
	}
	/************************/


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




    return 0;
}

int MyApp::postRender(){

	camera.computeFrustum();
	auto it = octTree.octStructTreeItemMap.begin();
	for (it; it != octTree.octStructTreeItemMap.end(); it++) {

		auto it2 = it->second->objects.begin();
		for (it2; it2 != it->second->objects.end(); it2++) {
			((rlgl::Object*)it2->data)->setInViewState(true);
		}
	}
	return 0;
}