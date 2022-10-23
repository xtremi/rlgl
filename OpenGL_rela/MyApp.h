#pragma once
#include "rlglBaseApp.h"
#include "rlOctree.h"

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

struct WorldObjects {
	rlgl::Object*			   worldPlane;
	std::vector<rlgl::Object*> cubes;
	std::vector<rlgl::Object*> axes;
};

struct UIobjects {
	std::vector<rlgl::Object*> aimCross;
};

class MyApp : public rlgl::BaseApp
{
public:

protected:
	rl::Octree	 octTree;
	rlgl::Camera secondaryCam;
	WorldObjects objects;
	UIobjects	 uiObjects;
	AssetIDs	 assetIDs;

	int prepareScene();
	int updateScene();
	int postRender();
	void prepareAssets();
	void createWorld();
	void createUI();
	void createBoxes();
	void createCSYS();
	void processInput(GLFWwindow* window);
};

/*!
	Function used in OctTree::callOnAllOctTreeObject
*/
namespace OctTreeFunc{
	void hideIfInFrustum(void* object, const rl::BoundingBox& bbox, void* frustumPtr);
	void unhide(void* object, const rl::BoundingBox& bbox, void*);
}

