#include "rlglMeshGenerator.h"
#include <glm\gtc\constants.hpp>
using namespace rlgl;



/*!
         x--- 
	    /
	   x----------
	   |
	   x-----
		\
		 x---x
*/
void MeshGenerator::generateSphere(
	std::shared_ptr<MeshVertexData> data,
	const rl::geom::Sphere& sphere,
	int nElementsAround,
	bool indexed,
	const glm::vec3& center)
{
	int nElPhi = 2 * (nElementsAround / 2);	//make even
	int nElTheta = nElPhi / 2;

	//Theta = [0, pi]
	//Phi   = [0,2pi]
	float angTheta = 0.f, angPhi = 0.f;
	float dAngPhi = glm::two_pi<float>() / (float)(nElPhi);
	float dAngTheta = dAngPhi;
	glm::vec3 coords, normal;

	//data->vertices.push_back(center.x);
	//data->vertices.push_back(center.y);
	//data->vertices.push_back(center.z - sphere.radius);

	angTheta = dAngTheta;

	for (int i = 0; i < (nElTheta - 1); i++) {

		for (int j = 0; j < nElPhi; j++) {

			normal = glm::vec3(
				glm::sin(angTheta) * glm::cos(angPhi),
				glm::sin(angTheta) * glm::sin(angPhi),
				glm::cos(angTheta));
			coords = sphere.radius * normal + center;
			data->vertices.push_back(coords.x);
			data->vertices.push_back(coords.y);
			data->vertices.push_back(coords.z);
			if(generateNormals){
				data->vertices.push_back(normal.x);
				data->vertices.push_back(normal.y);
				data->vertices.push_back(normal.z);
			}
			angPhi += dAngPhi;
		}

		angTheta += dAngTheta;
		angPhi = 0.f;
	}

	//data->vertices.push_back(center.x);
	//data->vertices.push_back(center.y);
	//data->vertices.push_back(center.z + sphere.radius);

	if (indexed) {
		generateSphereIndices(data->indices, nElementsAround);
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
void MeshGenerator::generateSphereIndices(std::vector<unsigned int>&indices, int nElementsAround) {

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
			indices.push_back(elIndices[0]);
			indices.push_back(elIndices[1]);
			indices.push_back(elIndices[2]);
			indices.push_back(elIndices[0]);
			indices.push_back(elIndices[2]);
			indices.push_back(elIndices[3]);
			elIndices[0]++;

		}
	}


}

