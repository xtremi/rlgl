#include "SampleApp.h"


SampleApp::SampleApp() {
    //glfwSetCursorPosCallback(window, SampleApp::mouse_callback);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static const float CURSOR_YAW_PITCH_SENSITIVITY = 0.25f;
void SampleApp::handleMouse(double xpos, double ypos)
{
    static bool firstMouse = true;
    static float lastX = 0.f;
    static float lastY = 0.f;
    static float yaw = 0.f, pitch = 0.f;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= CURSOR_YAW_PITCH_SENSITIVITY;
    yoffset *= CURSOR_YAW_PITCH_SENSITIVITY;

    yaw -= xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f){
        pitch = 89.0f;
    }
    if (pitch < -89.0f){
        pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.z = sin(glm::radians(pitch)); 
    camera.setFront(glm::normalize(front));

    
}

static const float CURSOR_MOVE_SPEED = 0.005f;
void SampleApp::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveForward(CURSOR_MOVE_SPEED);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveBackward(CURSOR_MOVE_SPEED);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveLeft(CURSOR_MOVE_SPEED);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveRight(CURSOR_MOVE_SPEED);
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    handleMouse(xpos, ypos);
    //glfwSetCursorPos(window, windowSize.x / 2, windowSize.y / 2);


}


void SampleApp::cleanUp() {
    scene.cleanUp();
}


int SampleApp::initializeWindow(int windowX, int windowY) {

    windowSize = glm::ivec2(windowX, windowY);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)

    window = glfwCreateWindow(windowSize.x, windowSize.y, appLabel.c_str(), NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        errmsg = "glfwCreateWindow() - Failed to create GLFW window";
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        errmsg = "gladLoadGLLoader() - Failed to initialize GLAD";
        return -2;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPos(window, windowSize.x /2, windowSize.y / 2);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
    glEnable(GL_DEPTH_TEST);
    return 0;
}

int SampleApp::prepareScene() {

    rlgl::primitive_mesh::plane_textureX10.initialize();
    rlgl::primitive_mesh::plane.initialize();
    rlgl::primitive_mesh::cube.initialize();

    uint64_t meshWorld = scene.addMesh(&rlgl::primitive_mesh::plane_textureX10);
    uint64_t meshCube = scene.addMesh(&rlgl::primitive_mesh::cube);
    uint64_t meshPlane = scene.addMesh(&rlgl::primitive_mesh::plane);


    rlgl::Material material1;
    material1.initialize("..\\data\\checker_grey.jpg", true);
    uint64_t material1ID = scene.addMaterial(material1);

    rlgl::Material material2;
    material2.initialize("..\\data\\box-texture.png", false);
    uint64_t material2ID = scene.addMaterial(material2); 


    rlgl::Shader shader1;
    shader1.initialize("vertexShaderSimple.vs", "fragmentShaderSimple.fs");
    shader1.setInt("textureID", material1.glID);
    uint64_t shader1ID = scene.addShader(shader1);


    objects.worldPlane = new rlgl::Object(meshWorld, shader1ID, material1ID);
    
    objects.worldPlane->modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
    objects.worldPlane->modelMatrix = glm::scale(objects.worldPlane->modelMatrix, glm::vec3(100.0f)); 
    scene.addObject(objects.worldPlane);


    for (int i = 0; i < 4; i++) {
        objects.squares.push_back(new rlgl::Object(meshPlane, shader1ID, material2ID));
        scene.addObject(objects.squares[i]);
    }
    for (int i = 0; i < 4; i++) {
        objects.cubes.push_back(new rlgl::Object(meshCube, shader1ID, material2ID));
        scene.addObject(objects.cubes[i]);
    }

    return 0;
}

int SampleApp::renderScene() {

    renderer.render(scene, camera);

    return 0;
}

int SampleApp::updateScene() {

    float curTime = glfwGetTime();

    float d = 2.5f;
    glm::vec3 positions[4] = {
        glm::vec3(-d, -d, 0.0f),
        glm::vec3( d, -d, 0.0f),
        glm::vec3( d,  d, 0.0f),
        glm::vec3(-d,  d, 0.0f)
    }; 

    glm::mat4 T, R, S;
    for (int i = 0; i < objects.squares.size(); i++) {

        T = glm::translate(glm::mat4(1.0f), positions[i] + glm::vec3(0.f, 0.f, 1.5f));
        R = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 0.f, 1.f));
        S = glm::scale(glm::mat4(1.0), glm::vec3(1.0f));
    
        objects.squares[i]->modelMatrix = T * R * S;

        T = glm::translate(glm::mat4(1.0f), positions[i] + glm::vec3(0.f, 0.f, 0.7f));
        R = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 0.f, 1.f));
        S = glm::scale(glm::mat4(1.0), glm::vec3(1.0f));

        objects.cubes[i]->modelMatrix = T * R * S;
    }


    return 0;
}