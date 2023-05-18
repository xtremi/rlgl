#pragma once
#include "Example5_App.h"
#include "rlOctree.h"
#include "rlglLodController.h"
#include "rlglObject.h"
#include "rlglBaseApp.h"

class TerrainQuadObject : public rlgl::Object {
public:
	TerrainQuadObject(rlgl::MeshPtr mesh, rlgl::ShaderPtr shader, int _level, rlgl::LODloc _loc) 
		: rlgl::Object(mesh, shader), level{_level}, loc{_loc}{}

	int level;
	rlgl::LODloc loc;
};


struct Assets {
	struct Meshes {
		rlgl::MeshPtr cube, square, terrainDummy;
	};
	struct Shaders {
		rlgl::ShaderPtr textured, colored, ui;

	};
	Meshes	  mesh;
	Shaders   shader;
};

struct WorldObjects {
	std::vector<rlgl::Object*> axes;
	std::vector<TerrainQuadObject*>  terrainLODquads;
};

struct UIobjects {
	std::vector<rlgl::Object*> aimCross;
};

class MyApp : public rlgl::BaseApp
{
public:
	MyApp(const std::string& assetDirectory);

protected:
	rl::Octree			octTree;
	rlgl::LODcontroller lodControl;
	rlgl::Camera		secondaryCam;

	WorldObjects  objects;
	UIobjects	  uiObjects;
	Assets		  assets;

	int prepareScene();						//inherited from BaseApp
	int updateScene();						//inherited from BaseApp
	int postRender();						//inherited from BaseApp
	void processInput(GLFWwindow* window);	//inherited from BaseApp

	//initialization
	void prepareAssets();	
	void createLODterrain();

	//update
	void updateTerrainLOD();

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

