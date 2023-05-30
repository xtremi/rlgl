#pragma once
#include "rlOctree.h"
#include "rlglObject.h"
#include "rlglBaseApp.h"

struct Assets {
	struct Meshes {
		rlgl::MeshPtr world, cubeTex, cube, frustum;
	};
	struct Materials {
		rlgl::MaterialPtr checker, box, boxMetal;
	};
	struct Shaders {
		rlgl::ShaderPtr textured, colored;

	};
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
};

struct WorldObjects {
	rlgl::Object*			   worldPlane;
	std::vector<rlgl::Object*> cubes;
	rlgl::Object*			   instObj;
	rlgl::Object*			   frustum;
};

class MyApp : public rlgl::BaseApp
{
public:
	MyApp(const std::string& assetDirectory);

protected:
	rl::Octree	 octTree;
	rlgl::Camera secondaryCam;

	WorldObjects  objects;
	Assets		  assets;
	rlgl::MeshPtr cubeTexInstMesh;
	rlgl::MeshPtr frustumMesh;

	int prepareScene() override;
	int updateScene() override;
	int postRender() override;
	void processInput(GLFWwindow* window) override;

	//initialization
	void prepareAssets();	
	void createWorld();
	void createBoxes();
	void createFrustumObject();

	//update
	void updateBoxes();

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

