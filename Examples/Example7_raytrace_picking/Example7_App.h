#pragma once
#include "rlglBaseApp.h"
#include "rlOctree.h"
#include "rlglBaseApp.h"

struct Assets {
	struct Meshes {
		rlgl::MeshPtr world, cubeTex, cube, square;
	};
	struct Materials {
		rlgl::MaterialPtr checker, box;
	};
	struct Shaders {
		rlgl::ShaderPtr textured, colored, ui;

	};
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
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
	MyApp(const std::string& assetDirectory);

protected:
	rl::Octree	 octTree;
	rlgl::Camera secondaryCam;
	WorldObjects objects;
	UIobjects	 uiObjects;
	Assets		 assets;

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

	//update
	void updateBoxes();
	void updateHitTestOctTree();

private:
	static const float BOX_WIDTH;
	bool octreeFrustumCullingON = true;
};

/*!
	Function used in OctTree::callOnAllOctTreeObject
*/
namespace OctTreeFunc{

	bool isInFrustum(rl::OctreeItem* octreeNode, void* frustumPtr);
	bool isInFrustum(const rl::BoundingBox& boundingBox, void* frustumPtr);
	void hide(void* object);
	void setHighlight(void* object);
	void setHighlightColorBlue(void* object);
	void setHighlightColorRed(void* object);
	void setHighlightColorGreen(void* object);
	void setNoHighlight(void* object);

	void hideIfOutsideFrustum(void* object, const rl::BoundingBox& bbox, void* frustumPtr);
	void unhide(void* object, const rl::BoundingBox& bbox, void*);
}

