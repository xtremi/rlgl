#pragma once
#include <glm/glm.hpp>

namespace rlgl{

class LightProperties {

public:
	LightProperties() = default;

	LightProperties(
		glm::vec3 _ambientStrength,
		glm::vec3 _diffuseStrength,
		glm::vec3 _specularStrength,
		float	  _shininess);

	void setProperties(
		glm::vec3 _ambientStrength,
		glm::vec3 _diffuseStrength,
		glm::vec3 _specularStrength,
		float	  _shininess);

	glm::vec3 ambientStrength = glm::vec3(0.2f);
	glm::vec3 diffuseStrength = glm::vec3(0.5f);
	glm::vec3 specularStrength = glm::vec3(5.5f);
	float shininess = 32.0f;

public:
	static LightProperties gold();
	static LightProperties jade();
	static LightProperties mettalic();
	static LightProperties standard();
};

}