#pragma once
#include "rlglBaseApp.h"

struct AssetIDs {
	struct Meshes {
		uint64_t cube, cubeTex, world, square;
	};
	struct Materials {
		uint64_t checker, box;
	};
	struct Shaders {
		uint64_t textured, colored, ui;

	};
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
};

class LightBox : public rlgl::Object {
public:
	LightBox(float intensity, uint64_t meshID, uint64_t shaderID);
protected:
	float _intensity = 1.0f;
};

struct WorldObjects {
	rlgl::Object*			   worldPlane;
	std::vector<rlgl::Object*> boxes;
	std::vector<rlgl::Object*> axes;
	rlgl::Object*			   instObj;
};

struct UIobjects {
	std::vector<rlgl::Object*> aimCross;
};

class MyApp : public rlgl::BaseApp
{
public:
	MyApp(const std::string& assetDirectory);

protected:
	WorldObjects objects;
	UIobjects	 uiObjects;
	AssetIDs	 assetIDs;

	int prepareScene();						//inherited from BaseApp
	int updateScene();						//inherited from BaseApp
	int postRender();						//inherited from BaseApp
	void processInput(GLFWwindow* window);	//inherited from BaseApp

	//initialization
	void prepareAssets();	
	void createWorld();
	void createUI();
	void createBoxes();
	void createLight();
	void createCSYS();

	//Update:
	void updateBoxes();
	void updateLight();


private:
	static const float BOX_WIDTH;
};
