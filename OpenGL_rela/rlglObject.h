#pragma once
#include <glm/glm.hpp>

namespace rlgl{

class Object
{
public:
	uint32_t meshID;
	uint32_t shaderID;
	uint32_t materialID;

	glm::mat4 modelMatrix;
};



}