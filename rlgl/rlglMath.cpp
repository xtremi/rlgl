#include "rlglMath.h"
#include  <cstdlib>

float rlgl::rand(float min, float max) {
	return min + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

glm::quat rlgl::fromToRotation(const glm::vec3& startDirection, const glm::vec3& endDirection) {

    const glm::vec3& crossProduct = glm::cross(startDirection, endDirection);

    float sineOfAngle = glm::length(crossProduct);

    float angle = glm::asin(sineOfAngle);
    glm::vec3 axis = crossProduct / sineOfAngle;

    glm::vec3 imaginary = glm::sin(angle / 2.0f) * axis;

    glm::quat result;
    result.w = glm::cos(angle / 2.0f);
    result.x = imaginary.x;
    result.y = imaginary.y;
    result.z = imaginary.z;

    return result;
}