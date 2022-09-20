#pragma once
#include "rlglShapeGeometry.h"
#include <glm/glm.hpp>

namespace rlgl{

class Object
{
public:
	Mesh* mesh;


	glm::mat4 modelMatrix;
};



}