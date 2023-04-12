#include "rlglMesh.h"

namespace rlgl {

	class MeshBank;

	class DefaultMeshes {
		friend MeshBank;

		static MeshPtr plane;
		static MeshPtr square;
		static MeshPtr plane_textureX10;
		static MeshPtr cube_tex;
		static MeshPtr cube;
		static MeshPtr cubeMap;
		static MeshPtr grass;
		static MeshPtr terrainDummy;
	};

}