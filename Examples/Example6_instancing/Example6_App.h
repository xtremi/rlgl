#pragma once
#include "rlglBaseApp.h"

struct Assets {
	struct Meshes {
		rlgl::MeshPtr world, cubeInst;
	};
	struct Materials {
		rlgl::MaterialPtr checker, boxMetal;
	};
	struct Shaders {
		rlgl::ShaderPtr textured, inst;

	};
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
};

struct WorldObjects {
	rlgl::Object*			   worldPlane;
	rlgl::Object*			   instObj;
};

class MyApp : public rlgl::BaseApp
{
public:
	MyApp(const std::string& assetDirectory);

protected:
	rlgl::Camera  secondaryCam;
	WorldObjects  objects;
	Assets		  assets;
	rlgl::MeshPtr cubeTexInstMesh;

	int prepareScene() override;
	int updateScene() override;
	int postRender() override;
	void processInput(GLFWwindow* window) override;

	void prepareAssets();	
	void createWorld();
	void createUI();
	void createBoxes();

private:
	static const float BOX_WIDTH;
};

