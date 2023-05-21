#pragma once
#include "rlglBaseApp.h"
#include "rlOctree.h"
#include "rlglBaseApp.h"

struct Assets {
	struct Meshes {
		rlgl::MeshPtr world, cubeTex;
	};
	struct Materials {
		rlgl::MaterialPtr checker, box;
	};
	struct Shaders {
		rlgl::ShaderPtr textured;

	};
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
};

struct WorldObjects {
	rlgl::Object*			   worldPlane;
	std::vector<rlgl::Object*> cubes;
};

class MyApp : public rlgl::BaseApp
{
public:
	MyApp(const std::string& assetDirectory);

protected:
	rl::Octree	 octTree;
	rlgl::Camera secondaryCam;
	WorldObjects objects;
	Assets		 assets;

	int prepareScene() override;
	int updateScene() override;
	int postRender() override;
	void processInput(GLFWwindow* window) override;

	void prepareAssets();	
	void createWorld();
	void createBoxes();

	void updateBoxes();
	void updateHitTestOctTree();

private:
	static const float BOX_WIDTH;
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

