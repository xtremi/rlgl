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

glm::mat4 Camera::pvMatrix() const {
	return projectionMatrix() * viewMatrix();
}

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
/*
	Eliminates any translation in the view matrix
	Used by sky cube map to keep centered.
*/
glm::mat4 Camera::viewMatrix_noTranslation() const {
	return glm::mat4(glm::mat3(viewMatrix()));
}

glm::vec3 Camera::lookVec() const {
	return front;
}
glm::vec3 Camera::sideVec() const {
	return glm::normalize(glm::cross(upVector, lookVec()));
}
glm::vec3 Camera::correctedUpVector() const {
	return glm::normalize(glm::cross(lookVec(), sideVec()));
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

void Camera::frustumCorners(
	std::vector<glm::vec3>& nearPlaneCoords,
	std::vector<glm::vec3>& farPlaneCoords)
{
	glm::vec3 upVectorC = correctedUpVector();

	glm::vec3 p = position + front * near;

	float fovY = glm::radians(fov);
	float fovX = glm::atan(glm::tan(fovY / 2.f) * aspectRatio) * 2.f;
	float nearPlaneHalfWidth = glm::sin(fovX / 2.) * near;
	float nearPlaneHalfHeight = glm::sin(fovY / 2.) * near;

	nearPlaneCoords.push_back(p - sideVec() * nearPlaneHalfWidth - upVectorC * nearPlaneHalfHeight);
	nearPlaneCoords.push_back(p + sideVec() * nearPlaneHalfWidth - upVectorC * nearPlaneHalfHeight);
	nearPlaneCoords.push_back(p + sideVec() * nearPlaneHalfWidth + upVectorC * nearPlaneHalfHeight);
	nearPlaneCoords.push_back(p - sideVec() * nearPlaneHalfWidth + upVectorC * nearPlaneHalfHeight);

	p = position + front * far*0.97f; //to be a little bit nearer than far (if not won't see)
	float farPlaneHalfWidth = glm::sin(fovX / 2.) * far;
	float farPlaneHalfHeight = glm::sin(fovY / 2.) * far;

	farPlaneCoords.push_back(p - sideVec() * farPlaneHalfWidth - upVectorC * farPlaneHalfHeight);
	farPlaneCoords.push_back(p + sideVec() * farPlaneHalfWidth - upVectorC * farPlaneHalfHeight);
	farPlaneCoords.push_back(p + sideVec() * farPlaneHalfWidth + upVectorC * farPlaneHalfHeight);
	farPlaneCoords.push_back(p - sideVec() * farPlaneHalfWidth + upVectorC * farPlaneHalfHeight);
}

void Camera::computeFrustum() {
	computeFrustum_method3();
}


/*!
Method based on my understanding.
(rotating front vector to find the direction of frustum edges)
This method also calculates correct (hopefully) positions of center of planes
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
	glm::vec3 upVectorC = correctedUpVector();
	float fovY = glm::radians(fov);
	float fovX = glm::atan(glm::tan(fovY / 2.f) * aspectRatio) * 2.f;
	fovX /= 2.f;
	fovY /= 2.f;

	frustum.near() = rl::Plane(front, position + front * near);
	frustum.far() = rl::Plane(-front, position + front * far);

	glm::vec3 side = sideVec();
	glm::vec3 Lvec = rl::rotateVec3(front, upVectorC, fovX);
	glm::vec3 Rvec = rl::rotateVec3(front, upVectorC, -fovX);
	glm::vec3 Tvec = rl::rotateVec3(front, side, -fovY);
	glm::vec3 Bvec = rl::rotateVec3(front, side, fovY);

	float nearDistanceFOV = near / glm::cos(fovX);
	float farDistanceFOV = far / glm::cos(fovX);

	frustum.left() = rl::Plane(
		glm::cross(upVectorC, -Lvec),
		position + (Lvec * nearDistanceFOV + Lvec * farDistanceFOV) / 2.f);

	frustum.right() = rl::Plane(
		glm::cross(upVectorC, Rvec),
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

/*!
Based on https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
slightly edited, I think because of definition of camera CSYS
I consider:
	X-direction to be out of camera (front / lookVec)
	Y-direction to be side vector (positive left)
	Z-direction up vector

	Sometimes online I see negative Z axis being out of camera (front / lookVec)
*/
void Camera::computeFrustum_method2(){
	glm::vec3 upVectorC = correctedUpVector();

	float fovY = glm::radians(fov);
	const float halfVSide = far * tanf(fovY * .5f);
	const float halfHSide = halfVSide * aspectRatio;
	const glm::vec3 frontMultFar = far * front;

	glm::vec3 sideL = sideVec();
	glm::vec3 sideR = -sideVec();

	frustum.near() = rl::Plane(
		front, 
		position + near * front);
	frustum.far() = rl::Plane(
		-front, 
		position + frontMultFar);

	frustum.left() = rl::Plane(
		glm::cross(frontMultFar - sideR * halfHSide, upVectorC),
		position);
	frustum.right() = rl::Plane(
		glm::cross(upVectorC,frontMultFar + sideR * halfHSide),
		position);
	//frustum.top() = rl::Plane(
	//	glm::cross(sideR, frontMultFar - upVector * halfVSide),
	//	position);
	frustum.top() = rl::Plane(
		glm::cross(sideL, frontMultFar + upVectorC * halfVSide),
		position);
	frustum.bottom() = rl::Plane(
		glm::cross(sideR, frontMultFar - upVectorC * halfVSide),
		position);
}

/*!
Based on https://stackoverflow.com/questions/12836967/extracting-view-frustum-planes-gribb-hartmann-method
*/
void Camera::computeFrustum_method3(){
	glm::mat4 mat = projectionMatrix() * viewMatrix();
	glm::vec4 leftN, rightN, bottomN, topN, nearN, farN;

	for (int i = 3; i--; ) leftN[i]		= mat[i][3] + mat[i][0];
	for (int i = 3; i--; ) rightN[i]	= mat[i][3] - mat[i][0];
	for (int i = 3; i--; ) bottomN[i]	= mat[i][3] + mat[i][1];
	for (int i = 3; i--; ) topN[i]		= mat[i][3] - mat[i][1];
	for (int i = 3; i--; ) nearN[i]		= mat[i][3] + mat[i][2];
	for (int i = 3; i--; ) farN[i]		= mat[i][3] - mat[i][2];

	//leftN  	= glm::normalize(leftN);
	//rightN 	= glm::normalize(rightN);
	//bottomN = glm::normalize(bottomN);
	//topN 	= glm::normalize(topN);
	//nearN	= glm::normalize(nearN);
	//farN	= glm::normalize(farN);

	frustum.near()	 = rl::Plane(nearN, position + front * near);
	frustum.far()	 = rl::Plane(farN, position + front * far);
	frustum.left()	 = rl::Plane(leftN,	 position);
	frustum.right()  = rl::Plane(rightN,  position);
	frustum.top()	 = rl::Plane(topN,	 position);
	frustum.bottom() = rl::Plane(bottomN, position);
}