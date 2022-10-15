#pragma once
#include "rlglBaseApp.h"
#include "rlOctree.h"

class MyApp : public rlgl::BaseApp
{
public:
	virtual int prepareScene();
	virtual int updateScene();
	virtual void processInput(GLFWwindow* window);

protected:
	rl::Octree octTree;
	void prepareAssets();
	void createWorld();
	void createUI();
	void createBoxes();
	void createCSYS();

	struct WorldObjects {
		rlgl::Object* worldPlane;
		std::vector<rlgl::Object*> cubes;
		std::vector<rlgl::Object*> axes;
		std::vector<rlgl::Object*> boundingBoxes;
	};

	struct UIobjects {
		std::vector<rlgl::Object*> aimCross;
	};

	WorldObjects objects;
	UIobjects uiObjects;

	struct AssetIDs {
		struct Meshes {
			uint64_t world, cubeTex, cube, square;
		};
		struct Materials {
			uint64_t checker, box;
		};
		struct Shaders {
			uint64_t textured, colored, ui;
			
		};
		Meshes mesh;
		Materials material;
		Shaders shader;
	};
	AssetIDs assetIDs;

};

