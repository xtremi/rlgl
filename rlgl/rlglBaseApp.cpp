#include "rlglBaseApp.h"

using namespace rlgl;


void rlgl::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

BaseApp::BaseApp(const std::string& assetDirectory) : _assetDirectory{ assetDirectory } {}


/*!*/
int BaseApp::init(int windowSizeX, int windowSizeY) {
    activeCamera = &camera;
    windowParams().size = glm::ivec2(windowSizeX, windowSizeY);
    camera.aspectRatio = windowParams().aspect();
    
	uiCamera.position = glm::vec3(0.f);
	uiCamera.front  = glm::vec3(0.f, 0.f, 1.f);
	uiCamera.upVector = glm::vec3(0.f, 1.f, 0.f);
	uiCamera.aspectRatio = windowParams().aspect();
    uiCamera.isOrthoGraphic = true;

    if (int err = initializeWindow()) {
        return err;
    }

    if (int err = prepareScene()) {
        return err;
    }

    return 0;
}

#include <iostream>
bool FPScontrol::process() {
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	if (deltaTime >= maxPeriod){

        FPS_values[currentFrame] = 1.0 / deltaTime;

		if(currentFrame == (n_frames - 1)){
            double sum = 0.;
            for (size_t i = 0; i < n_frames; i++) {
                sum += FPS_values[i];
            }
            double fpsAvg = sum / (double)n_frames;
			std::cout << "FPS = " << fpsAvg << std::endl;
            currentFrame = 0;
		}
		lastTime = currentTime;
        currentFrame++;
		return true;
	}
    return false;
}

/*!*/
int BaseApp::loopIteration() {
  
	if (!fpsControl.process()) {
		return 0;
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processInput(_window);

    if (int err = updateScene()) {
        return err;
    }
    if (int err = renderScene()) {
        return err;
    }
	if(int err = postRender()){
		return err;
	}
    glfwSwapBuffers(_window);
    glfwPollEvents();



    return 0;
}

bool BaseApp::windowClosed() {
    return glfwWindowShouldClose(_window);
}



static const float CURSOR_YAW_PITCH_SENSITIVITY = 0.25f;
void BaseApp::handleMouse(double xpos, double ypos)
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
    activeCamera->setFront(glm::normalize(front));    
}

static const float CURSOR_MOVE_SPEED = 0.15f;
void BaseApp::processInput(GLFWwindow* window)
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
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        camera.fov -= 0.1;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        camera.fov += 0.1;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    handleMouse(xpos, ypos);
    //glfwSetCursorPos(window, windowSize.x / 2, windowSize.y / 2);
}


void BaseApp::cleanUp() {
    scene.cleanUp();
}


int BaseApp::initializeWindow() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwParams().glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwParams().glVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWmonitor** monitors = nullptr;
	int nMonitors = 0;
	monitors = glfwGetMonitors(&nMonitors);
	if (nMonitors == 0) {
		glfwTerminate();
		errmsg = "glfwGetMonitors() - found 0 monitors";
		return (int)error_code::GLFW_NO_MONITORS;
	}
    
    _window = glfwCreateWindow(
		windowParams().size.x, windowParams().size.y, 
		windowParams().label.c_str(), 
		NULL, NULL);

    if (_window == NULL)
    {
        glfwTerminate();
        errmsg = "glfwCreateWindow() - Failed to create GLFW window";
        return (int)error_code::GLFW_CREATE_WINDOW;
    }
    glfwMakeContextCurrent(_window);
   
    
    //if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    if (!gladLoadGL(glfwGetProcAddress))
    {
        errmsg = "gladLoadGLLoader() - Failed to initialize GLAD";
        return (int)error_code::GLAD_LOAD_GL_LOADER;
    }

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glfwSetCursorPos(_window, windowParams().size.x /2, windowParams().size.y / 2);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    
    //For transperancy:
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return 0;
}



int BaseApp::renderScene() {

    renderer.render(scene, *activeCamera);
    renderer.render(uiScene, uiCamera);

    return 0;
}


void DemoApp::processInput(GLFWwindow* window) {
    BaseApp::processInput(window);
}


int DemoApp::prepareScene() {

    rlgl::primitive_mesh::plane_textureX10->initialize();
    rlgl::primitive_mesh::plane->initialize();
    rlgl::primitive_mesh::cube_tex->initialize();
    rlgl::primitive_mesh::cube->initialize();

    rlgl::MeshPtr meshWorld = rlgl::primitive_mesh::plane_textureX10;
    rlgl::MeshPtr meshCubeTex = rlgl::primitive_mesh::cube_tex;
    rlgl::MeshPtr meshPlane = rlgl::primitive_mesh::plane;
    rlgl::MeshPtr meshCube = rlgl::primitive_mesh::cube;


    MaterialPtr material1 = std::make_shared<Textured2dMaterial>(_assetDirectory + "\\textures\\checker_grey.jpg", true);
    MaterialPtr material2 = std::make_shared<Textured2dMaterial>(_assetDirectory + "\\textures\\box-texture.png", false);

    rlgl::ShaderPtr shader1 = std::make_shared<StandardShader>(
        _assetDirectory + "\\shaders\\object.vs", 
        _assetDirectory + "\\shaders\\object.fs");
    rlgl::ShaderPtr shader2 = std::make_shared<StandardShader>(
        _assetDirectory + "\\shaders\\object_col.vs", 
        _assetDirectory + "\\shaders\\object_col.fs");

    objects.worldPlane = new rlgl::Object(meshWorld, shader1, material1);

    objects.worldPlane->setPosition(glm::vec3(0.f, 0.f, 0.f));
    objects.worldPlane->setScale(glm::vec3(100.0f));
    scene.addObject(objects.worldPlane);


    for (int i = 0; i < 4; i++) {
        objects.squares.push_back(new rlgl::Object(meshPlane, shader1, material2));
        scene.addObject(objects.squares[i]);
    }
    for (int i = 0; i < 4; i++) {
        objects.cubes.push_back(new rlgl::Object(meshCubeTex, shader1, material2));
        scene.addObject(objects.cubes[i]);
    }

    return 0;
}

int DemoApp::updateScene() {

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
    
        objects.squares[i]->setModelMatrix(T * R * S);

        T = glm::translate(glm::mat4(1.0f), positions[i] + glm::vec3(0.f, 0.f, 0.7f));
        R = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 0.f, 1.f));
        S = glm::scale(glm::mat4(1.0), glm::vec3(1.0f));

        objects.cubes[i]->setModelMatrix(T * R * S);
    }


    return 0;
}