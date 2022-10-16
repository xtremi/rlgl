#include "rlglCamera.h"
#include "rlMath.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace rlgl;

rl::Plane& Frustum::near() {
	return planes[0];
}
rl::Plane& Frustum::far() {
	return planes[1];
}
rl::Plane& Frustum::top() {
	return planes[2];
}
rl::Plane& Frustum::bottom() {
	return planes[3];
}
rl::Plane& Frustum::left() {
	return planes[4];
}
rl::Plane& Frustum::right() {
	return planes[5];
}

/***************************************************/

glm::mat4 Camera::projectionMatrix() const {
	if(!isOrthoGraphic){
		return glm::perspective(glm::radians(fov), aspectRatio, near, far);
	}
	else {
		return glm::ortho(-1.f, 1.f, -1.f, 1.f);
	}
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
	//position.z = 1.f;
	//lookAt += lookVec() * distance;
}
void Camera::moveBackward(float distance) {
	moveForward(-distance);
}
void Camera::moveLeft(float distance) {
	position += sideVec() * distance;
	//position.z = 1.f;
	//lookAt += sideVec() * distance;
}
void Camera::moveRight(float distance) {
	moveLeft(-distance);
}


void Camera::computeFrustum() {
	computeFrustum_method1();
}


/*!
        x
		/|
	D /  |
	/    | near plane
  / fov  |
x----N---|-------->  front vec
pos

N = near distance
D = distance from pos to intersection of near plane with side planes of frustum
(same for left, right, top and bottom planes)

*/
void Camera::computeFrustum_method1(){

	frustum.near() = rl::Plane(front, position + front * near);
	frustum.far() = rl::Plane(-front, position + front * far);

	glm::vec3 side = sideVec();
	glm::vec3 Lvec = rl::rotateVec3(front, upVector, glm::radians(fov));
	glm::vec3 Rvec = rl::rotateVec3(front, upVector, -glm::radians(fov));
	glm::vec3 Tvec = rl::rotateVec3(front, side, glm::radians(fov));
	glm::vec3 Bvec = rl::rotateVec3(front, side, -glm::radians(fov));

	float nearDistanceFOV = near / glm::cos(glm::radians(fov));
	float farDistanceFOV = far / glm::cos(glm::radians(fov));

	frustum.left() = rl::Plane(
		position + (Lvec * nearDistanceFOV + Lvec * farDistanceFOV) / 2.f,
		glm::cross(upVector, -Lvec)
	);

	frustum.right() = rl::Plane(
		position + (Rvec * nearDistanceFOV + Rvec * farDistanceFOV) / 2.f,
		glm::cross(upVector, Rvec)
	);

	frustum.top() = rl::Plane(
		position + (Tvec * nearDistanceFOV + Tvec * farDistanceFOV) / 2.f,
		glm::cross(side, Tvec)
	);

	frustum.bottom() = rl::Plane(
		position + (Bvec * nearDistanceFOV + Bvec * farDistanceFOV) / 2.f,
		glm::cross(side, -Bvec)
	);

}
void Camera::computeFrustum_method2(){
}
void Camera::computeFrustum_method3(){
}