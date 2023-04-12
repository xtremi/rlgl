#include "rlglMeshBank.h"
using namespace rlgl;

MeshPtr MeshBank::getInitializedMesh(MeshPtr mesh) {
	if (!mesh->isInitialized()) {
		mesh->initialize();
	}
	return mesh;
}

MeshPtr MeshBank::defaultPlane() {
	return getInitializedMesh(DefaultMeshes::plane);
}
MeshPtr MeshBank::defaultSquare() {
	return getInitializedMesh(DefaultMeshes::square);
}
MeshPtr MeshBank::defaultPlane_textureX10() {
	return getInitializedMesh(DefaultMeshes::plane_textureX10);
}
MeshPtr MeshBank::defaultCube_tex() {
	return getInitializedMesh(DefaultMeshes::cube_tex);
}
MeshPtr MeshBank::defaultCube() {
	return getInitializedMesh(DefaultMeshes::cube);
}
MeshPtr MeshBank::defaultCubeMap() {
	return getInitializedMesh(DefaultMeshes::cubeMap);
}
MeshPtr MeshBank::defaultGrass() {
	return getInitializedMesh(DefaultMeshes::grass);
}
MeshPtr MeshBank::defaultTerrainDummy() {
	return getInitializedMesh(DefaultMeshes::terrainDummy);
}


