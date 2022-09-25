#include "rlglCamera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace rlgl;

glm::mat4 Camera::projectionMatrix() const {
	return glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

glm::mat4 Camera::viewMatrix() const {
	return glm::lookAt(position, position + front, upVector);
}
glm::vec3 Camera::lookVec() const {
	return front;
}
glm::vec3 Camera::sideVec() const {
	return glm::cross(upVector, lookVec());
}

void Camera::setFront(const glm::vec3& _front) {
	front = _front;
}

void Camera::moveForward(float distance) {
	position += lookVec() * distance;
	position.z = 1.f;
	//lookAt += lookVec() * distance;
}
void Camera::moveBackward(float distance) {
	moveForward(-distance);
}
void Camera::moveLeft(float distance) {
	position += sideVec() * distance;
	position.z = 1.f;
	//lookAt += sideVec() * distance;
}
void Camera::moveRight(float distance) {
	moveLeft(-distance);
}