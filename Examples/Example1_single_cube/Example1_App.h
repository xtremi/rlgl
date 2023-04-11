#pragma once
#include "rlglBaseApp.h"

struct Assets {
	struct Meshes {
		rlgl::MeshPtr cube, cubeTex, world, square;
	};
	struct Shaders {
		rlgl::ShaderPtr textured, colored, ui;
	};
	struct Materials {
		rlgl::MaterialPtr checker, box;
	};
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
};

struct WorldObjects {
	rlgl::Object*			   worldPlane;
	rlgl::Object* 			   cube;
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
	Assets		 assets;

	int prepareScene();						//inherited from BaseApp
	int updateScene();						//inherited from BaseApp

	//initialization
	void prepareAssets();	
	void createWorld();
	void createUI();
	void createBox();
	void createCSYS();


private:
	static const float BOX_WIDTH;
};
