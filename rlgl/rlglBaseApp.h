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
#include "rlglErrors.h"

namespace rlgl{

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	struct GLFWparams {
		int glVersionMajor = 4;
		int glVersionMinor = 0;
	};
	struct WindowParams {
		std::string label = "rlgl BaseApp v1.0.0";
		glm::ivec2  size = glm::ivec2(800, 600);
		glm::vec3   backgroundColor = glm::vec3(0.f);

		float aspect() {
			return (float)size.x / (float)size.y;
		}
	};


class BaseApp {

public:
	BaseApp();

	int init(int windowSizeX, int windowSizeY);
	int loopIteration();
	void cleanUp();
	bool windowClosed();


	GLFWwindow* window() {
		return _window;
	}
	GLFWparams& glfwParams() {
		return _glfwParams;
	}
	WindowParams& windowParams() {
		return _windowParams;
	}

protected:	
	GLFWparams	   _glfwParams;
	WindowParams   _windowParams;
	GLFWwindow*	   _window = nullptr;
	rlgl::Renderer renderer;
	rlgl::Camera   camera;
	rlgl::Scene	   scene;
	std::string    errmsg = "";

	void handleMouse(double xposIn, double yposIn);
	int initializeWindow();
	int renderScene();

	virtual int prepareScene() = 0;
	virtual int updateScene() = 0;
	virtual void processInput(GLFWwindow* window);
};


class DemoApp : public BaseApp {
public:
	virtual int prepareScene();
	virtual int updateScene();
	virtual void processInput(GLFWwindow* window);
	
protected:
	class WorldObjects {
	public:
		rlgl::Object* worldPlane;
		std::vector<rlgl::Object*> squares;
		std::vector<rlgl::Object*> cubes;

		std::vector<rlgl::Object*> axes;
	};
	WorldObjects objects;

};


}