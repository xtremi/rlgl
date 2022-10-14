#include "MyApp.h"
#include "rlMath.h"
#include <iostream>

void MyApp::processInput(GLFWwindow* window) {
    BaseApp::processInput(window);
}

int MyApp::prepareScene() {

    rlgl::primitive_mesh::plane_textureX10.initialize();
    rlgl::primitive_mesh::plane.initialize();
    rlgl::primitive_mesh::cube_tex.initialize();
    rlgl::primitive_mesh::cube.initialize();
    rlgl::primitive_mesh::square.initialize();

    uint64_t meshWorld = scene.addMesh(&rlgl::primitive_mesh::plane_textureX10);
    uint64_t meshCubeTex = scene.addMesh(&rlgl::primitive_mesh::cube_tex);
    uint64_t meshCube = scene.addMesh(&rlgl::primitive_mesh::cube);
    	
    rlgl::Material material1;
    material1.initialize("..\\data\\textures\\checker_grey.jpg", true);
    uint64_t material1ID = scene.addMaterial(material1);

    rlgl::Material material2;
    material2.initialize("..\\data\\textures\\box-texture.png", false);
    uint64_t material2ID = scene.addMaterial(material2);


    rlgl::Shader shader1;
    shader1.initialize("..\\data\\shaders\\object.vs", "..\\data\\shaders\\object.fs");
    shader1.setInt("textureID", material1.glID);
    uint64_t shader1ID = scene.addShader(shader1);


    rlgl::Shader shader2;
    shader2.initialize("..\\data\\shaders\\object_col.vs", "..\\data\\shaders\\object_col.fs");
    uint64_t shader2ID = scene.addShader(shader2);


    //World plane:
    objects.worldPlane = new rlgl::Object(meshWorld, shader1ID, material1ID);
	
    objects.worldPlane->modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
    objects.worldPlane->modelMatrix = glm::scale(objects.worldPlane->modelMatrix, glm::vec3(100.0f));
    scene.addObject(objects.worldPlane);

	//UI:
	uint64_t uiSquareMesh = uiScene.addMesh(&rlgl::primitive_mesh::square);

	rlgl::Shader shader3_ui;
	shader3_ui.initialize("..\\data\\shaders\\ui_element.vs", "..\\data\\shaders\\ui_element.fs");
	uint64_t shader3ID_ui = uiScene.addShader(shader3_ui);

    uiObjects.aimCross.push_back(new rlgl::Object(uiSquareMesh, shader3ID_ui, INT64_MAX));
    uiObjects.aimCross.push_back(new rlgl::Object(uiSquareMesh, shader3ID_ui, INT64_MAX));
    uiObjects.aimCross[0]->modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.2f));
    uiObjects.aimCross[0]->modelMatrix = glm::scale(uiObjects.aimCross[0]->modelMatrix, glm::vec3(0.2f, 0.01f, 1.f));
	uiObjects.aimCross[1]->modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, 0.2f));
	uiObjects.aimCross[1]->modelMatrix = glm::scale(uiObjects.aimCross[1]->modelMatrix, glm::vec3(0.01f, 0.2f, 1.f));
	uiObjects.aimCross[0]->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
	uiObjects.aimCross[1]->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
	uiScene.addObject(uiObjects.aimCross[0]);
	uiScene.addObject(uiObjects.aimCross[1]);


    //Boxes:
    rl::OctreeStruct octStruct({ 0.f, 0.f, 0.f }, 100.f, 5);
	octTree = rl::Octree(octStruct);

    int nBoxes = 500;
    glm::vec3 boxPos;
	rl::BoundingBox bbox;
    float boxSize = 2.f;
    for (int i = 0; i < nBoxes; i++) {

        boxPos  = glm::vec3(rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f));
		bbox = rl::BoundingBox::createCubeBoundingBox(boxPos, boxSize);

        objects.cubes.push_back(new rlgl::Object(meshCubeTex, shader1ID, material2ID));
        objects.cubes[i]->modelMatrix = glm::translate(glm::mat4(1.f), boxPos);
        objects.cubes[i]->modelMatrix = glm::scale(objects.cubes[i]->modelMatrix, glm::vec3(boxSize));
		rl::OctreeItem* item = octTree.addObject((void*)objects.cubes[i], bbox);
        scene.addObject(objects.cubes[i]);
    }

    float axesL = 5.f;
    float axesW = 0.2f;
    std::vector<glm::vec3> axesDir({ glm::vec3(1.f, 0.f, 0.f),glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f , 0.f, 1.f) });
    std::vector<glm::vec3> axesScales({ glm::vec3(axesL, axesW, axesW),glm::vec3(axesW, axesL, axesW), glm::vec3(axesW, axesW, axesL) });
    std::vector<glm::vec4> axesColor({ glm::vec4(1.f, 0.f, 0.f, 1.f),glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec4(0.f, 0.f, 1.f, 1.f) });
    for (int i = 0; i < axesDir.size(); i++) {
        objects.axes.push_back(new rlgl::Object(meshCube, shader2ID, material2ID));
        objects.axes[i]->setColor(axesColor[i]);
        objects.axes[i]->modelMatrix = glm::translate(glm::mat4(1.f), axesDir[i] * axesL / 2.f);  
        objects.axes[i]->modelMatrix = glm::scale(objects.axes[i]->modelMatrix, axesScales[i]);
        scene.addObject(objects.axes[i]);
    }
    
    return 0;
}

static rlgl::Object* lastHitObj = nullptr;
int MyApp::updateScene() {

    double curTime = glfwGetTime();

	rl::Ray hitRay(camera.lookVec(), camera.position);
	 
	rlgl::Object* hitObj = nullptr;
	if (octTree.hitTest(hitRay, (void**)&hitObj)) {
		if (lastHitObj && (lastHitObj != hitObj)) lastHitObj->setHighlight(false);

		hitObj->modelMatrix = glm::rotate(hitObj->modelMatrix, 0.01f, glm::vec3(0.f, 0.f, 1.f));
		hitObj->setHighlight(true);	
		lastHitObj = hitObj;
	}
	else {
		if (lastHitObj) lastHitObj->setHighlight(false);
	}

    return 0;
}