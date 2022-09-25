#pragma once
#include <glm/glm.hpp>

namespace rlgl{

class Object
{
public:
	Object(uint64_t meshID, uint64_t shaderID, uint64_t materialID);

public:
	uint64_t meshID;
	uint64_t shaderID;
	uint64_t materialID;

	glm::mat4 modelMatrix;
};



}