#pragma once
#include "rlOctree.h"
#include "rlglLodController.h"
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
		rlgl::MeshPtr cube, terrainDummy;
	};
	struct Shaders {
		rlgl::ShaderPtr colored;

	};
	Meshes	  mesh;
	Shaders   shader;
};

struct WorldObjects {
	std::vector<TerrainQuadObject*>  terrainLODquads;
};

class MyApp : public rlgl::BaseApp
{
public:
	MyApp(const std::string& assetDirectory);

protected:
	rlgl::Camera		secondaryCam;
	rl::Octree			octTree;
	rlgl::LODcontroller lodControl;
	WorldObjects		objects;
	Assets				assets;

	int prepareScene() override;
	int updateScene() override;
	int postRender() override;
	void processInput(GLFWwindow* window) override;

	void prepareAssets();	
	void createLODterrain();

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

