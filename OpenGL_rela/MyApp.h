#pragma once
#include "rlglBaseApp.h"
#include "rlOctStructTree.h"

class MyApp : public rlgl::BaseApp
{
public:
	virtual int prepareScene();
	virtual int updateScene();
	virtual void processInput(GLFWwindow* window);

protected:
	rl::OctStructTree octTree;
	rlgl::Scene uiScene;

	class WorldObjects {
	public:
		rlgl::Object* worldPlane;
		std::vector<rlgl::Object*> cubes;
		std::vector<rlgl::Object*> axes;

		std::vector<rlgl::Object*> boundingBoxes;
	};
	class UIobjects {
	public:
		std::vector<rlgl::Object*> aimCross;
	};
	WorldObjects objects;
	UIobjects uiObjects;

};

