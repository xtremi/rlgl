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
void processInput(GLFWwindow* window);



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
	GLFWwindow* window = nullptr;
	glm::ivec2	   windowSize = glm::ivec2(800, 600);

	std::string appLabel = "SampleApp v0.0.1";
	std::string errmsg = "";

	glm::vec3 backgroundColor = glm::vec3(0.f);
};