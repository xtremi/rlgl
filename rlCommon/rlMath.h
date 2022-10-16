#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace rl {
	float rand(float min, float max);

	glm::quat fromToRotation(const glm::vec3& startDirection, const glm::vec3& endDirection);

	glm::vec3 rotateVec3(const glm::vec3& vec, const glm::vec3& axis, float angle);

}

