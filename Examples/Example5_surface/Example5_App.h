#pragma once
#include "rlglBaseApp.h"

struct Assets {
	struct Meshes {
		rlgl::MeshPtr cube, cubeMap;
		rlgl::MeshPtr world, square;
		rlgl::MeshPtr plane1, plane2;
	};
	struct Shaders {
		rlgl::ShaderPtr textured, texturedLight, texturedLightMat;
		rlgl::ShaderPtr colored, coloredLightMat;
		rlgl::ShaderPtr ui;
		rlgl::ShaderPtr skyBox;
	};
	struct Materials {
		rlgl::MaterialPtr checker; 
		rlgl::MaterialPtr metalic;
		rlgl::MaterialPtr skyCubeMap1;
	};
	
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
};


struct WorldObjects {
	rlgl::Object*			   worldPlane, *skyBox;
	std::vector<rlgl::Object*> axes;
	std::vector<rlgl::Object*> lightBoxes;
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

	int prepareScene() override;
	int updateScene() override;
	int postRender() override;
	void processInput(GLFWwindow* window) override;

	//initialization
	void prepareAssets();	
	void createWorld();
	void createUI();
	void createSurface();
	void createCSYS();
	void createSkyBox();

private:
	static const float PLANE_SIDE_LENGTH;
};
