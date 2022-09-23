#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "rlglScene.h"
#include "rlglRenderer.h"
#include "rlglShader.h"
#include "rlglMesh.h"
#include "rlglMaterial.h"
#include "rlglObject.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

struct rlglContext {
    GLuint VBO, VAO, EBO;
    rlgl::Shader shader;

    GLuint textures[10];
};

bool triangleTest01_init(rlglContext& ctxt);
bool triangleTest02_init(rlglContext& ctxt);
void triangleTest01_renderRoutine(const rlglContext& ctxt);
void triangleTest02_renderRoutine(const rlglContext& ctxt);

class SampleApp {
	
public:
	int initializeWindow(int windowX, int windowY);
	int prepareScene();
	int renderScene();
	int updateScene();


	void cleanUp();

	GLFWwindow* getWindow() {
		return window;
	}

	rlgl::Renderer renderer;

private:
	rlgl::Scene	   scene;
	GLFWwindow*	   window = nullptr;
	glm::ivec2	   windowSize = glm::ivec2(800, 600);

	std::string appLabel = "SampleApp v0.0.1";
	std::string errmsg = "";

	glm::vec3 backgroundColor = glm::vec3(0.f);
};

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
	rlgl::primitive_mesh::cube.initialize();

	uint64_t mesh1 = scene.addMesh(&rlgl::primitive_mesh::plane);
	uint64_t mesh2 = scene.addMesh(&rlgl::primitive_mesh::cube);
	
	rlgl::Shader shader1;
	shader1.initialize("vertexShaderSimple.vs", "fragmentShaderSimple.fs");
	shader1.setInt("textureID", 0);
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

	glm::mat4 translM = glm::translate(glm::mat4(1.5f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 rotM = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 1.f, 1.f));
	glm::mat4 scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.6f));
	obj1->modelMatrix = translM * scaleM * rotM;



	return 0;
}


int main(int argc, char* argv[]) {

	SampleApp app;
	app.renderer = rlgl::Renderer(800.f / 600.f);

	if (int err = app.initializeWindow(800, 600)) {
		return err;
	}
    
	if (int err = app.prepareScene()) {
		return err;
	}


    while (!glfwWindowShouldClose(app.getWindow()))
    {
        processInput(app.getWindow());
        
		app.updateScene();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (int err = app.renderScene()) {
			return err;
		}

        glfwSwapBuffers(app.getWindow());
        glfwPollEvents();
    }

	app.cleanUp();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}


/*
bool triangleTest02_init(rlglContext& ctxt) {
    ctxt.shader.initialize("vertexShaderSimple.vs", "fragmentShaderSimple.fs");

    glGenVertexArrays(1, &ctxt.VAO);
    glBindVertexArray(ctxt.VAO);

    rlgl::cube.vertices.genBuffer();
    rlgl::cube.vertices.bindBuffer(GL_ARRAY_BUFFER);
    rlgl::cube.vertices.bufferData(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    //position:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    //texture:
    int width, height, nrChannels;
    unsigned char* imgData = stbi_load(
        "..\\data\\checker.jpg", &width, &height, &nrChannels, 0);

    glGenTextures(1, &ctxt.textures[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctxt.textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
    ctxt.shader.setInt("textureID", 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imgData);



    return true;
}


bool triangleTest01_init(rlglContext& ctxt) {
    
    ctxt.shader.initialize("vertexShaderSimple.vs", "fragmentShaderSimple.fs");

    glGenVertexArrays(1, &ctxt.VAO);
    glBindVertexArray(ctxt.VAO);
    
    rlgl::plane.vertices.genBuffer();
    rlgl::plane.vertices.bindBuffer(GL_ARRAY_BUFFER);
    rlgl::plane.vertices.bufferData(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    //position:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //indices:
    rlgl::plane.indices.genBuffer();
    rlgl::plane.indices.bindBuffer(GL_ELEMENT_ARRAY_BUFFER);
    rlgl::plane.indices.bufferData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    //texture:
    int width, height, nrChannels;
    unsigned char* imgData = stbi_load(
        "C:\\coding\\my_projects\\LearnOpenGL\\OpenGL_rela\\data\\checker.jpg", &width, &height, &nrChannels, 0);
    
    glGenTextures(1, &ctxt.textures[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctxt.textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
    ctxt.shader.setInt("textureID", 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imgData);
    return true;
}


void triangleTest02_renderRoutine(const rlglContext& ctxt) {
    float width = 800.f, height = 600.f;

    float curTime = glfwGetTime();

    ctxt.shader.use();

    glm::mat4 projM = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 viewM = glm::lookAt(glm::vec3(-2.f, -2.f, 1.5f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f));
    ctxt.shader.setMat4x4("projection", projM);
    ctxt.shader.setMat4x4("view", viewM);

    glm::mat4 translM = glm::translate(glm::mat4(1.5f), glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 rotM = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 1.f, 1.f));
    glm::mat4 scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.6f));
    glm::mat4 modelM = translM * scaleM * rotM;
    ctxt.shader.setMat4x4("model", modelM);

    //glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    //glBindTexture(GL_TEXTURE_2D, ctxt.textures[0]);
    glBindVertexArray(ctxt.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    translM = glm::translate(glm::mat4(1.f), glm::vec3(2.f, 0.f, 1.f));
    modelM = translM * scaleM * rotM;
    ctxt.shader.setMat4x4("model", modelM);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void triangleTest01_renderRoutine(const rlglContext& ctxt) {
    float width = 800.f, height = 600.f;

    float curTime = glfwGetTime();

    ctxt.shader.use();

    //glm::mat4 ortho = glm::ortho(0.0f, width, 0.0f, height, 0.1f, 100.0f);
    glm::mat4 projM = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    //glm::mat4 viewM = glm::translate(glm::mat4(1.f), glm::vec3(-0.f, -0.2f, -1.f));
    glm::mat4 viewM = glm::lookAt(glm::vec3(-2.f, -2.f, 1.5f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f));
    ctxt.shader.setMat4x4("projection", projM);
    ctxt.shader.setMat4x4("view", viewM);

    glm::mat4 rotM = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 scaleM = glm::scale(glm::mat4(2.0f), glm::vec3(0.6f));
    glm::mat4 modelM = scaleM * rotM;
    ctxt.shader.setMat4x4("model", modelM);

    //glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    //glBindTexture(GL_TEXTURE_2D, ctxt.textures[0]);
    glBindVertexArray(ctxt.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    modelM = glm::translate(scaleM, glm::vec3(1.5f, 0.f, 0.f));
    modelM = modelM * rotM;
    ctxt.shader.setMat4x4("model", modelM);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}*/




