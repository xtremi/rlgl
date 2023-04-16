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
	nElementsAround = 2 * (nElementsAround / 2);	//make even
	int nElementsUp = nElementsAround / 2;

	float angV = 0.f, angH = 0.f;
	float dAngH = glm::two_pi<float>() / (float)nElementsAround;
	float dAngV = dAngH / 2.0f;
	glm::vec3 coords;

	for (int i = 0; i < nElementsUp; i++) {

		for (int j = 0; j < nElementsAround; j++) {

			coords = glm::vec3(
				glm::sin(angV) + glm::cos(angH),
				glm::sin(angV) + glm::sin(angH),
				glm::cos(angV));
			coords = sphere.radius * coords + center;
			data->vertices.push_back(coords.x);
			data->vertices.push_back(coords.y);
			data->vertices.push_back(coords.z);

			angH += dAngH;

			//Bottom and top has only one coordinate
			if (i == 0 || i == (nElementsUp - 1)) {
				break;
			}
		}

		angV += dAngV;
	}

	if (indexed) {
		generateSphereIndices(data->indices, nElementsAround);
	}

}

void MeshGenerator::generateSphereIndices(std::vector<unsigned int>&indices, int nElementsAround) {

	nElementsAround = 2 * (nElementsAround / 2);	//make even
	int nElementsUp = nElementsAround / 2;

	int elIndices[4] = { 0,0,0,0 };

	for (int i = 0; i < nElementsUp; i++) {

		for (int j = 0; j < nElementsAround; j++) {

			if(i > 0){
				elIndices[0]++;
				elIndices[1] = elIndices[0] + 1;
				elIndices[2] = elIndices[1] + nElementsAround;
				elIndices[3] = elIndices[2] - 1;
			}
			else {
				elIndices[0] = 0;
				elIndices[1]++;
				elIndices[2] = elIndices[1] + 1;
			}
			
			indices.push_back(elIndices[0]);
			indices.push_back(elIndices[1]);
			indices.push_back(elIndices[2]);
			
			if(i > 0){
				indices.push_back(elIndices[0]);
				indices.push_back(elIndices[2]);
				indices.push_back(elIndices[3]);
			}

		}
	}


}

