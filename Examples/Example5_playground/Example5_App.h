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
		rlgl::MeshPtr world, cubeTex, cube, square, cubeInst, terrainDummy, frustum;
		rlgl::MeshPtr grass;
	};
	struct Materials {
		rlgl::MaterialPtr checker, box, boxMetal;
	};
	struct Shaders {
		rlgl::ShaderPtr textured, colored, ui, inst;
		rlgl::ShaderPtr grass;

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

