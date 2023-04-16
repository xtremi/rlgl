#include "rlglMeshGenerator.h"
#include <glm\gtc\constants.hpp>
using namespace rlgl;



void MeshVertexData::addVertexVec3(const glm::vec3& vec){
	vertices.push_back(vec.x);
	vertices.push_back(vec.y);
	vertices.push_back(vec.z);
}
void MeshVertexData::addVertexVec2(const glm::vec2& vec) {
	vertices.push_back(vec.x);
	vertices.push_back(vec.y);
}
void MeshVertexData::addIndices(int elIndices[4]) {
	addIndices(elIndices[0], elIndices[1], elIndices[2]);
	addIndices(elIndices[0], elIndices[2], elIndices[3]);
}
void MeshVertexData::addIndices(int triIndex1, int triIndex2, int triIndex3) {
	indices.push_back(triIndex1);
	indices.push_back(triIndex2);
	indices.push_back(triIndex3);
}

void MeshGenerator::generateSphere(
	std::shared_ptr<MeshVertexData> data,
	const rl::geom::Sphere& sphere,
	int nElementsAround,
	bool indexed,
	const glm::vec3& center)
{
	generateSphereVertices(data, sphere, nElementsAround, indexed, center);

	if (indexed) {
		generateSphereIndices(data, nElementsAround);
	}

}


void MeshGenerator::generateSphereVertices(
	std::shared_ptr<MeshVertexData> data,
	const rl::geom::Sphere&			sphere,
	int								nElementsAround,
	bool							indexed,
	const glm::vec3&				center)
{
	int nElPhi = 2 * (nElementsAround / 2);	//make even
	int nElTheta = nElPhi / 2;

	//Theta = [0, pi]
	//Phi   = [0,2pi]
	float angTheta = 0.f, angPhi = 0.f;
	float dAngPhi = glm::two_pi<float>() / (float)(nElPhi);
	float dAngTheta = dAngPhi;
	glm::vec3 coords, normal;

	angTheta = dAngTheta;

	for (int i = 0; i < (nElTheta - 1); i++) {

		for (int j = 0; j < nElPhi; j++) {

			normal = glm::vec3(
				glm::sin(angTheta) * glm::cos(angPhi),
				glm::sin(angTheta) * glm::sin(angPhi),
				glm::cos(angTheta));
			coords = sphere.radius * normal + center;

			data->addVertexVec3(coords);
			if(generateNormals) {
				data->addVertexVec3(normal);
			}
			angPhi += dAngPhi;
		}

		angTheta += dAngTheta;
		angPhi = 0.f;
	}

	glm::vec3 topPos = center + sphere.radius * glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 botPos = center - sphere.radius * glm::vec3(0.f, 0.f, 1.f);

	data->addVertexVec3(topPos);
	if (generateNormals) {
		data->addVertexVec3(glm::vec3(0.f, 0.f, 1.f));
	}
	data->addVertexVec3(botPos);
	if (generateNormals) {
		data->addVertexVec3(glm::vec3(0.f, 0.f, -1.f));
	}
}

/*

	nElementsAround = 6

    6   7   8   9  10  11   6
	x---x---x---x---x---x---x
	|   |   |   |   |   |   |
	x---x---x---x---x---x---x
    0   1   2   3   4   5   0

	0,1,7,6,  1,2,8,7 ... 5,0,6,11

*/
void MeshGenerator::generateSphereIndices(std::shared_ptr<MeshVertexData> data, int nElementsAround) {

	int nElPhi = 2 * (nElementsAround / 2);	//make even
	int nElTheta = nElPhi / 2;

	int elIndices[4] = { 0,0,0,0 };

	for (int i = 0; i < (nElTheta - 2); i++) {

		for (int j = 0; j < nElPhi; j++) {

			elIndices[1] = elIndices[0] + 1;
			elIndices[2] = elIndices[1] + nElPhi;
			elIndices[3] = elIndices[2] - 1;
			if(j == (nElPhi - 1)) {
				elIndices[1] -= nElPhi;
				elIndices[2] -= nElPhi;
			}
			data->addIndices(elIndices);
			elIndices[0]++;
		}
	}

	int lastVertexIndex = elIndices[3];

	generateTriangleFanRow(data, nElPhi,
		lastVertexIndex + 1,	//index of top of shere vertex
		0);						//first index of sphere

	generateTriangleFanRow(data, nElPhi,
		lastVertexIndex + 2,	//index of bot of sphere vertex
		elIndices[1]);			//index of the first vertex of last row

}

void MeshGenerator::generateTriangleFanRow(
	std::shared_ptr<MeshVertexData> data,
	int nElements,
	int indexCenter,
	int firstIndex)
{
	int index2 = firstIndex, index3;
	for (int j = 0; j < nElements; j++) {
		index3 = index2 + 1;
		if (j == (nElements- 1)) {
			index3 -= nElements;
		}
		data->addIndices(indexCenter, index2, index3);
		index2++;
	}
}

