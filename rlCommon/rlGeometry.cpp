#include "rlGeometry.h"
using namespace rl;

Ray::Ray(
	const glm::vec3& direction,
	const glm::vec3& origin) :
	_origin{ origin }
{
	setDirection(direction);
}

void Ray::setDirection(const glm::vec3& direction) {
	_direction = glm::normalize(direction);
	
	glm::vec3 dirNonNull = _direction;
	for (int i = 0; i < 3; i++) {
		if (dirNonNull[i] == 0.f || dirNonNull[i] == -0.f) {
			dirNonNull[i] = FLT_MIN;
		}
	}

	_dir_inv = glm::vec3(
		1.f / dirNonNull.x,
		1.f / dirNonNull.y,
		1.f / dirNonNull.z);
}
void Ray::setOrigin(const glm::vec3& origin) {
	_origin = origin;
}