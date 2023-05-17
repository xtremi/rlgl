#pragma once
#include "rlglMesh.h"
#include "rlglDefaultMeshes.h"

namespace rlgl {

	class MeshBank {

	public:
		/*Default meshes will always return the same mesh object.*/
		static MeshPtr defaultPlane();
		static MeshPtr defaultSquare();
		static MeshPtr defaultPlane_textureX10();
		static MeshPtr defaultCube_tex();
		static MeshPtr defaultCube();
		static MeshPtr defaultCubeMap();
		static MeshPtr defaultGrass();
		static MeshPtr defaultTerrainDummy();
		
		static MeshPtr cube();

	private:
		static MeshPtr getInitializedMesh(MeshPtr mesh);
		static DefaultMeshes defaultMeshes;		
	};


}