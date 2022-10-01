#include "MyApp.h"
#include "rlglMath.h"


void MyApp::processInput(GLFWwindow* window) {
    BaseApp::processInput(window);
}


int MyApp::prepareScene() {

    rlgl::primitive_mesh::plane_textureX10.initialize();
    rlgl::primitive_mesh::plane.initialize();
    rlgl::primitive_mesh::cube_tex.initialize();
    rlgl::primitive_mesh::cube.initialize();

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
    shader1.initialize("..\\data\\shaders\\object_col.vs", "..\\data\\shaders\\object_col.fs");
    uint64_t shader2ID = scene.addShader(shader1);


    //World plane:
    objects.worldPlane = new rlgl::Object(meshWorld, shader1ID, material1ID);

    objects.worldPlane->modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
    objects.worldPlane->modelMatrix = glm::scale(objects.worldPlane->modelMatrix, glm::vec3(100.0f));
    scene.addObject(objects.worldPlane);

    //Boxes:
    rl::OctStruct octStruct({ 0.f, 0.f, 0.f }, 100.f, 5);
    octTree.octStruct = octStruct;


    int maxPoints = 100;
    glm::vec3 boxPos;
    rl::OctCoord bboxMin, bboxMax; 
    float boxSize = 1.f;
    std::vector<void*> allAddedObjects;
    for (int i = 0; i < maxPoints; i++) {

        boxPos  = glm::vec3(rlgl::rand(-50.f, 50.f), rlgl::rand(-50.f, 50.f), rlgl::rand(-50.f, 50.f));

        bboxMin.x = boxPos.x - boxSize / 2.f;
        bboxMin.y = boxPos.y - boxSize / 2.f;
        bboxMin.z = boxPos.z - boxSize / 2.f;

        bboxMax.x = boxPos.x + boxSize / 2.f;
        bboxMax.y = boxPos.y + boxSize / 2.f;
        bboxMax.z = boxPos.z + boxSize / 2.f;

        objects.cubes.push_back(new rlgl::Object(meshCubeTex, shader1ID, material2ID));
        objects.cubes[i]->modelMatrix = glm::translate(glm::mat4(1.f), boxPos);
        objects.cubes[i]->modelMatrix = glm::scale(objects.cubes[i]->modelMatrix, glm::vec3(boxSize));

        octTree.addObject((void*)objects.cubes[i], bboxMin, bboxMax); 

        scene.addObject(objects.cubes[i]);
    }


    return 0;
}

int MyApp::updateScene() {

    float curTime = glfwGetTime();

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


    return 0;
}