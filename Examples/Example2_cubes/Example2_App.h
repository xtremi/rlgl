#pragma once
#include "rlglBaseApp.h"

struct Assets {
	struct Meshes {
		rlgl::MeshPtr cube, cubeTex, cubeMap;
		rlgl::MeshPtr world, square;
	};
	struct Shaders {
		rlgl::ShaderPtr textured, texturedLight, texturedLightMat;
		rlgl::ShaderPtr colored, coloredLightMat;
		rlgl::ShaderPtr ui;
		rlgl::ShaderPtr skyBox;
	};
	struct Materials {
		rlgl::MaterialPtr checker; 
		rlgl::MaterialPtr box, boxJade, boxGold, jade, gold;
		rlgl::MaterialPtr metalic;
		rlgl::MaterialPtr skyCubeMap1;
	};
	
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
};


struct WorldObjects {
	rlgl::Object*			   worldPlane, *skyBox;
	std::vector<rlgl::Object*> boxes;
	std::vector<rlgl::Object*> axes;
	rlgl::Object*			   lightBox;
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
	Assets		 assets;

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
