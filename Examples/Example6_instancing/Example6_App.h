#pragma once
#include "rlglBaseApp.h"
#include "rlOctree.h"
#include "rlglObject.h"
#include "rlglBaseApp.h"

struct Assets {
	struct Meshes {
		rlgl::MeshPtr world, cubeTex, cube, square, cubeInst;
	};
	struct Materials {
		rlgl::MaterialPtr checker, box, boxMetal;
	};
	struct Shaders {
		rlgl::ShaderPtr textured, colored, ui, inst;

	};
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
};

struct WorldObjects {
	rlgl::Object*			   worldPlane;
	std::vector<rlgl::Object*> axes;
	rlgl::Object*			   instObj;
};

class MyApp : public rlgl::BaseApp
{
public:
	MyApp(const std::string& assetDirectory);

protected:
	rlgl::Camera secondaryCam;

	WorldObjects objects;
	Assets	 assets;
	rlgl::MeshPtr cubeTexInstMesh;
	rlgl::MeshPtr frustumMesh;

	int prepareScene();						//inherited from BaseApp
	int updateScene();						//inherited from BaseApp
	int postRender();						//inherited from BaseApp
	void processInput(GLFWwindow* window);	//inherited from BaseApp

	//initialization
	void prepareAssets();	
	void createWorld();
	void createUI();
	void createBoxes();

private:
	static const float BOX_WIDTH;
	bool octreeFrustumCullingON = true;
};

