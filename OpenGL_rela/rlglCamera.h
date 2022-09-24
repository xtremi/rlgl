#pragma once
#include <glm/glm.hpp>
namespace rlgl{
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

	glm::vec3 position = glm::vec3(0.f, 0.f, 10.f);
	glm::vec3 lookAt   = glm::vec3(0.f);
	glm::vec3 upVector = glm::vec3(0.f, 0.f, 1.f);
	float fov  = 45.f;
	float near = 0.1f;
	float far  = 100.f;
	float aspectRatio = 800.f / 600.f;

};

}