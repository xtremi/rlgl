#pragma once
#include <glad/gl.h>
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
#include "rlglConfig.h"

namespace rlgl{

	#define NO_MATERIAL nullptr

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	struct GLFWparams {
		int glVersionMajor = 4;
		int glVersionMinor = 0;
	};
	struct WindowParams {
		std::string label = "rlgl BaseApp v1.0.0";
		glm::ivec2  size = glm::ivec2(800, 600);

		float aspect() {
			return (float)size.x / (float)size.y;
		}
	};


class FPScontrol{
public:
	bool process();
	
private:
	const double maxFPS = 60.;
	const double maxPeriod = 1.0 / maxFPS;
	double lastTime = 0.0, currentTime, deltaTime;
	static const size_t n_frames = 20;
	double FPS_values[n_frames];
	size_t currentFrame = 0;
	double renderTime = 0.;

};

class BaseApp {

public:
	BaseApp(const std::string& assetDirectory, bool usingGlfw = true);
    ~BaseApp(){}

	int init(int windowSizeX, int windowSizeY, GLADloadfunc gladLoadFunction = nullptr);
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
	bool	   	   _usingGlfw;
	FPScontrol	   fpsControl;
	GLFWparams	   _glfwParams;
	WindowParams   _windowParams;
	GLFWwindow*	   _window = nullptr;
	rlgl::Renderer renderer;
	rlgl::Camera*  activeCamera;
	rlgl::Camera   camera;
	rlgl::Camera   uiCamera;
	rlgl::Scene	   scene;
	rlgl::Scene	   uiScene;
	std::string    errmsg = "";


	void handleMouse(double xposIn, double yposIn);
	int initializeWindow(GLADloadfunc gladLoadFunction = nullptr);
	int initializeWindowGlfw();
	int renderScene();

	virtual int prepareScene() = 0;
	virtual int updateScene() = 0;
	virtual void processInput(GLFWwindow* window);
	virtual int postRender() { return 0; }
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
