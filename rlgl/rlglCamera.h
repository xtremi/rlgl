#pragma once
#include <glm/glm.hpp>
#include "rlGeometry.h"

namespace rlgl{

class Frustum {
public:

	rl::Plane& near();
	rl::Plane& far();
	rl::Plane& top();
	rl::Plane& bottom();
	rl::Plane& left();
	rl::Plane& right();

	rl::Plane planes[6];
};

class Camera
{
public:
	glm::mat4 projectionMatrix() const;
	glm::mat4 viewMatrix() const;
	glm::vec3 lookVec() const;
	glm::vec3 sideVec() const;
	
	void setFront(const glm::vec3& front);
	void moveForward(float distance);
	void moveBackward(float distance);
	void moveLeft(float distance);
	void moveRight(float distance);

	void computeFrustum();

	Frustum frustum;
	glm::vec3 position = glm::vec3(-5.f, -5.f, 1.f);
	glm::vec3 front   = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 upVector = glm::vec3(0.f, 0.f, 1.f);
	float fov  = 45.f;
	float near = 0.1f;
	float far  = 300.f;
	float aspectRatio = 800.f / 600.f;

	bool isOrthoGraphic = false;

	

};

}