#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "rlglScene.h"
#include "rlglRenderer.h"
#include "rlglShader.h"
#include "rlglMesh.h"
#include "rlglMaterial.h"
#include "rlglObject.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);


class WorldObjects {
public:
	rlgl::Object* worldPlane;
	std::vector<rlgl::Object*> squares;
	std::vector<rlgl::Object*> cubes;
};

class SampleApp {

public:
	SampleApp();

	int initializeWindow(int windowX, int windowY);
	int prepareScene();
	int renderScene();
	int updateScene();

	void processInput(GLFWwindow* window);


	void cleanUp();

	GLFWwindow* getWindow() {
		return window;
	}

	rlgl::Renderer renderer;
	rlgl::Camera   camera;

private:	
	void handleMouse(double xposIn, double yposIn);

	rlgl::Scene	scene;
	GLFWwindow* window = nullptr;
	glm::ivec2	windowSize = glm::ivec2(800, 600);

	std::string appLabel = "SampleApp v0.0.2";
	std::string errmsg = "";

	glm::vec3 backgroundColor = glm::vec3(0.f);

private:
	WorldObjects objects;

};