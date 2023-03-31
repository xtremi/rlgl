#pragma once
#include "rlglBaseApp.h"
#include "rlOctree.h"
#include "rlglLodController.h"

class TerrainQuadObject : public rlgl::Object {
public:
	TerrainQuadObject(uint64_t meshID, uint64_t shaderID, int _level, rlgl::LODloc _loc) 
		: rlgl::Object(meshID, shaderID, INT64_MAX), level{_level}, loc{_loc}{}

	int level;
	rlgl::LODloc loc;
};


struct Assets {
	struct Meshes {
		uint64_t world, cubeTex, cube, square, cubeInst, terrainDummy, frustum;
		uint64_t grass;
	};
	struct Materials {
		uint64_t checker, box, boxMetal;
	};
	struct Shaders {
		uint64_t textured, colored, ui, inst;
		uint64_t grass;

	};
	Meshes	  mesh;
	Materials material;
	Shaders   shader;
};

struct WorldObjects {
	rlgl::Object*			   worldPlane;
	std::vector<rlgl::Object*> cubes;
	std::vector<rlgl::Object*> axes;
	rlgl::Object*			   instObj;

	std::vector<TerrainQuadObject*>  terrainLODquads;
	std::vector<rlgl::Object*> grass;

	rlgl::Object* frustum;
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
	rlgl::Camera secondaryCam;

	WorldObjects objects;
	UIobjects	 uiObjects;
	Assets	 assets;
	rlgl::Mesh	 cubeTexInstMesh;
	rlgl::Mesh	 frustumMesh;

	int prepareScene();						//inherited from BaseApp
	int updateScene();						//inherited from BaseApp
	int postRender();						//inherited from BaseApp
	void processInput(GLFWwindow* window);	//inherited from BaseApp

	//initialization
	void prepareAssets();	
	void createWorld();
	void createUI();
	void createBoxes();
	void createLODterrain();
	void createCSYS();
	void createFrustumObject();
	void createGrass();

	//update
	void updateBoxes();
	void updateHitTestOctTree();
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

