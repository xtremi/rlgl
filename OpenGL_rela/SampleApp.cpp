#include "SampleApp.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
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

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
    glEnable(GL_DEPTH_TEST);
    return 0;
}

int SampleApp::prepareScene() {

    rlgl::primitive_mesh::plane.initialize();
    //rlgl::primitive_mesh::cube.initialize();

    uint64_t mesh1 = scene.addMesh(&rlgl::primitive_mesh::plane);
    //uint64_t mesh2 = scene.addMesh(&rlgl::primitive_mesh::cube);

    rlgl::Shader shader1;
    shader1.initialize("vertexShaderSimple.vs", "fragmentShaderSimple.fs");
    shader1.setInt("textureID", 1);
    uint64_t shader1ID = scene.addShader(shader1);

    rlgl::Material material1;
    material1.initialize("..\\data\\checker.jpg");
    uint64_t material1ID = scene.addMaterial(material1);

    rlgl::Object* obj1 = new rlgl::Object(mesh1, shader1ID, material1ID);	//change to smart pointer
    uint64_t obj1ID = scene.addObject(obj1);

    return 0;
}

int SampleApp::renderScene() {

    renderer.render(scene);

    return 0;
}

int SampleApp::updateScene() {

    float curTime = glfwGetTime();

    rlgl::Object* obj1 = scene.object(0);

    glm::mat4 translM = glm::translate(glm::mat4(0.0f), glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 rotM = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 scaleM = glm::scale(glm::mat4(1.0), glm::vec3(2.6f));
    obj1->modelMatrix = translM * scaleM * rotM;


    return 0;
}