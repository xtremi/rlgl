#include "MyApp.h"
#include "rlMath.h"
#include <iostream>

void MyApp::processInput(GLFWwindow* window) {
    BaseApp::processInput(window);
}

static std::string OBJ_ADDRESS_TO_DEBUG = "";
static rlgl::Object* OBJ_TO_DEBUG = nullptr;
static rl::OctStructTreeItem* OCT_TREE_ITEM_TO_DEBUG = nullptr;
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
    //objects.worldPlane = new rlgl::Object(meshWorld, shader1ID, material1ID);
	//
    //objects.worldPlane->modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
    //objects.worldPlane->modelMatrix = glm::scale(objects.worldPlane->modelMatrix, glm::vec3(100.0f));
    //scene.addObject(objects.worldPlane);

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
    rl::OctStruct octStruct({ 0.f, 0.f, 0.f }, 100.f, 5);
	octTree = rl::OctStructTree(octStruct);

	/*rl::BoundingBox bboxes[4]; glm::vec4 bboxesColors[4];
	octStruct.localBoundingBox("642", bboxes[0]);  bboxesColors[0] = glm::vec4(1.f, 0.f, 0.f, 0.2f);
	octStruct.localBoundingBox("643", bboxes[1]);  bboxesColors[1] = glm::vec4(0.f, 1.f, 0.f, 0.2f);
	octStruct.localBoundingBox("644", bboxes[2]);	 bboxesColors[2] = glm::vec4(0.f, 0.f, 1.f, 0.2f);
	octStruct.localBoundingBox("645", bboxes[3]);	 bboxesColors[3] = glm::vec4(1.f, 1.f, 0.f, 0.2f);
	for (int i = 0; i < 4; i++) {
		rlgl::Object* octTreeItemObj = new rlgl::Object(meshCube, shader2ID, material2ID);
		octTreeItemObj->modelMatrix = glm::translate(glm::mat4(1.f), bboxes[i].center());
		octTreeItemObj->modelMatrix = glm::scale(octTreeItemObj->modelMatrix, bboxes[i].size());
		octTreeItemObj->setColor(bboxesColors[i]);

		scene.addObject(octTreeItemObj);
	}*/

    int maxPoints = 100;
    glm::vec3 boxPos;
	rl::BoundingBox bbox;
    float boxSize = 1.f;
    std::vector<void*> allAddedObjects;
	int objectToDebug = 98;
    for (int i = 0; i < maxPoints; i++) {

        boxPos  = glm::vec3(rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f), rl::rand(-50.f, 50.f));
		bbox = rl::BoundingBox::createCubeBoundingBox(boxPos, boxSize);

        objects.cubes.push_back(new rlgl::Object(meshCubeTex, shader1ID, material2ID));
        objects.cubes[i]->modelMatrix = glm::translate(glm::mat4(1.f), boxPos);
        objects.cubes[i]->modelMatrix = glm::scale(objects.cubes[i]->modelMatrix, glm::vec3(boxSize));

        rl::OctStructTreeItem* item = octTree.addObject((void*)objects.cubes[i], bbox);
		if (i == objectToDebug) {
			OBJ_ADDRESS_TO_DEBUG = item->address;
			OBJ_TO_DEBUG = objects.cubes[i];
			OCT_TREE_ITEM_TO_DEBUG = item;
			std::cout << "OBJ_ADDRESS_DEBUG: " + item->address << std::endl;
		}

        scene.addObject(objects.cubes[i]);
    }

    int bi = 0;
    auto it = octTree.octStructTreeItemMap.begin();
    for (it; it != octTree.octStructTreeItemMap.end(); it++) {

		if(false){
			rl::BoundingBox octTreeItemBB = it->second->boundingBox;
			rlgl::Object* octTreeItemObj = new rlgl::Object(meshCube, shader2ID, material2ID);
			octTreeItemObj->modelMatrix = glm::translate(glm::mat4(1.f), octTreeItemBB.center());
			octTreeItemObj->modelMatrix = glm::scale(octTreeItemObj->modelMatrix, octTreeItemBB.size());
			octTreeItemObj->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.2f));

			scene.addObject(octTreeItemObj);
		}

        auto it2 = it->second->objects.begin();
        for (it2; it2 != it->second->objects.end(); it2++) {

			rl::BoundingBox bb = it2->bbox;        
			glm::vec3 center = bb.center();
            glm::vec3 size = bb.size();

            objects.boundingBoxes.push_back(new rlgl::Object(meshCube, shader2ID, material2ID));
			if (it->second == OCT_TREE_ITEM_TO_DEBUG){
				objects.boundingBoxes[bi]->setColor(glm::vec4(0.9f, 0.0f, 0.0f, 0.6f));				
			}
			else {
				objects.boundingBoxes[bi]->setColor(glm::vec4(0.9f, 0.9f, 0.9f, 0.6f));
			}
            objects.boundingBoxes[bi]->modelMatrix = glm::translate(glm::mat4(1.f), center);
            objects.boundingBoxes[bi]->modelMatrix = glm::scale(objects.boundingBoxes[bi]->modelMatrix, size*1.25f);
            scene.addObject(objects.boundingBoxes[bi++]);
        }
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
    
    //shader2.use();
    //shader2.setVec3("color", glm::vec3(1.f, 0.1f, 0.1f));

    return 0;
}

static int hitCount = 0;
int MyApp::updateScene() {

    float curTime = glfwGetTime();

	rl::Ray hitRay(camera.lookVec(), camera.position);
	 
	rlgl::Object* hitObj = nullptr;
	if (octTree.hitTest(hitRay, (void**)&hitObj)) {
		std::cout << "hit: " << hitCount++ << std::endl;
		hitObj->modelMatrix = glm::rotate(hitObj->modelMatrix, 0.01f, glm::vec3(0.f, 0.f, 1.f));
	}

    //float d = 2.5f;
    //glm::vec3 positions[4] = {
    //    glm::vec3(-d, -d, 0.0f),
    //    glm::vec3(d, -d, 0.0f),
    //    glm::vec3(d,  d, 0.0f),
    //    glm::vec3(-d,  d, 0.0f)
    //};
    //
    //glm::mat4 T, R, S;
    //for (int i = 0; i < objects.cubes.size(); i++) {
    //
    //    T = glm::translate(glm::mat4(1.0f), positions[i]);
    //    R = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 0.f, 1.f));
    //    S = glm::scale(glm::mat4(1.0), glm::vec3(1.0f));
    //
    //    objects.cubes[i]->modelMatrix = T * R * S;
    //}

    /*objects.axes[0]->modelMatrix = glm::translate(glm::mat4(1.f), camera.position + 4.f * glm::vec3(camera.front));

    glm::vec3 direction = camera.front;
    glm::vec3 sideVec = camera.sideVec();

    glm::quat orientationQuat = rlgl::fromToRotation(glm::vec3(1.0f, 0.f, 0.f), camera.front);
    objects.axes[0]->modelMatrix *= glm::toMat4(orientationQuat);

    objects.axes[0]->modelMatrix = glm::scale(objects.axes[0]->modelMatrix, glm::vec3(3.0f, 0.2f, 0.2f));*/
    


    return 0;
}