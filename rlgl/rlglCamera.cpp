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


fovx vs fovy
fov_x  = atan(tan(fov_y/2) * aspect) * 2

*/
void Camera::computeFrustum_method1(){

	float fovY = glm::radians(fov);
	float fovX = glm::atan(glm::tan(fovY / 2.f) * aspectRatio) * 2.f;
	fovX /= 2.f;
	fovY /= 2.f;

	frustum.near() = rl::Plane(front, position + front * near);
	frustum.far() = rl::Plane(-front, position + front * far);

	glm::vec3 side = sideVec();
	glm::vec3 Lvec = rl::rotateVec3(front, upVector, fovX);
	glm::vec3 Rvec = rl::rotateVec3(front, upVector, -fovX);
	glm::vec3 Tvec = rl::rotateVec3(front, side, -fovY);
	glm::vec3 Bvec = rl::rotateVec3(front, side, fovY);

	float nearDistanceFOV = near / glm::cos(fovX);
	float farDistanceFOV = far / glm::cos(fovX);

	frustum.left() = rl::Plane(
		glm::cross(upVector, -Lvec),
		position + (Lvec * nearDistanceFOV + Lvec * farDistanceFOV) / 2.f);

	frustum.right() = rl::Plane(
		glm::cross(upVector, Rvec),
		position + (Rvec * nearDistanceFOV + Rvec * farDistanceFOV) / 2.f);

	nearDistanceFOV = near / glm::cos(fovY);
	farDistanceFOV = far / glm::cos(fovY);

	frustum.top() = rl::Plane(
		glm::cross(side, Tvec),
		position + (Tvec * nearDistanceFOV + Tvec * farDistanceFOV) / 2.f);

	frustum.bottom() = rl::Plane(
		glm::cross(side, -Bvec),
		position + (Bvec * nearDistanceFOV + Bvec * farDistanceFOV) / 2.f);

}
void Camera::computeFrustum_method2(){

	const float halfVSide = far * tanf(fov * .5f);
	const float halfHSide = halfVSide * aspectRatio;
	const glm::vec3 frontMultFar = far * front;

	glm::vec3 sideR = -sideVec();

	frustum.near() = rl::Plane(front, position + near * front);
	frustum.far() = rl::Plane(-front, (position + frontMultFar));

	frustum.right() = rl::Plane(
		glm::cross(upVector,frontMultFar + sideR * halfHSide),
		position);

	frustum.left() = rl::Plane(
		glm::cross(frontMultFar - sideR * halfHSide, upVector),
		position);
	frustum.top() = rl::Plane(
		glm::cross(sideR, frontMultFar - upVector * halfVSide),
		position);
	frustum.bottom() = rl::Plane(
		glm::cross(frontMultFar + upVector * halfVSide, sideR),
		position);
}
void Camera::computeFrustum_method3(){
}