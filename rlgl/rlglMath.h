#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace rlgl {
	float rand(float min, float max);

	glm::quat fromToRotation(const glm::vec3& startDirection, const glm::vec3& endDirection);

}

