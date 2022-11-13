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


struct AssetIDs {
	struct Meshes {
		uint64_t world, cubeTex, cube, square, cubeInst, terrainDummy;
	};
	struct Materials {
		uint64_t checker, box, boxMetal;
	};
	struct Shaders {
		uint64_t textured, colored, ui, inst;

	};
	Meshes mesh;
	Materials material;
	Shaders shader;
};

struct WorldObjects {
	rlgl::Object*			   worldPlane;
	std::vector<rlgl::Object*> cubes;
	std::vector<rlgl::Object*> axes;
	rlgl::Object*			   instObj;

	std::vector<TerrainQuadObject*>  terrainLODquads;
};

struct UIobjects {
	std::vector<rlgl::Object*> aimCross;
};

class MyApp : public rlgl::BaseApp
{
public:

protected:
	rl::Octree			octTree;
	rlgl::LODcontroller lodControl;
	rlgl::Camera secondaryCam;

	WorldObjects objects;
	UIobjects	 uiObjects;
	AssetIDs	 assetIDs;
	rlgl::Mesh	 cubeTexInstMesh;

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

	//update
	void updateCubes();
	void updateHitTestOctTree();
	void updateTerrainLOD();

private:
	static const float BOX_WIDTH;
};

/*!
	Function used in OctTree::callOnAllOctTreeObject
*/
namespace OctTreeFunc{
	void hideIfInFrustum(void* object, const rl::BoundingBox& bbox, void* frustumPtr);
	bool isInFrustumOrHide(void* object, const rl::BoundingBox& bbox, void* frustumPtr);
	void unhide(void* object, const rl::BoundingBox& bbox, void*);
}

