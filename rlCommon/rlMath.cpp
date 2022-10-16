#include "rlMath.h"
#include  <cstdlib>

float rl::rand(float min, float max) {
	return min + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

glm::quat rl::fromToRotation(const glm::vec3& startDirection, const glm::vec3& endDirection) {

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

glm::vec3 rl::rotateVec3(const glm::vec3& vec, const glm::vec3& axis, float angle) {
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.f), angle, axis);
    glm::vec4 vec4 = rotMat * glm::vec4(vec, 1.f);
    return glm::vec3(vec4.x, vec4.y, vec4.z);
}