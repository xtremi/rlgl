#include "rlglCamera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace rlgl;

glm::mat4 Camera::projectionMatrix() const {
	return glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

glm::mat4 Camera::viewMatrix() const {
	return glm::lookAt(position, lookAt, upVector);
}
glm::vec3 Camera::lookVec() const {
	return glm::normalize(lookAt - position);
}
glm::vec3 Camera::sideVec() const {
	return glm::cross(upVector, lookVec());
}

void Camera::setFront(const glm::vec3& front) {
	lookAt = position + front;
}

void Camera::moveForward(float distance) {
	position += lookVec() * distance;
	lookAt += lookVec() * distance;
}
void Camera::moveBackward(float distance) {
	moveForward(-distance);
}
void Camera::moveLeft(float distance) {
	position += sideVec() * distance;
	lookAt += sideVec() * distance;
}
void Camera::moveRight(float distance) {
	moveLeft(-distance);
}