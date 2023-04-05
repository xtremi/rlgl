#include "rlglLightProperties.h"
using namespace rlgl;

LightProperties::LightProperties(
	glm::vec3 _ambientStrength,
	glm::vec3 _diffuseStrength,
	glm::vec3 _specularStrength,
	float	  _shininess)
{
	setProperties(_ambientStrength, _diffuseStrength, _specularStrength, _shininess);
}

void LightProperties::setProperties(
	glm::vec3 _ambientStrength,
	glm::vec3 _diffuseStrength,
	glm::vec3 _specularStrength,
	float	  _shininess)
{
	ambientStrength = _ambientStrength;
	diffuseStrength = _diffuseStrength;
	specularStrength = _specularStrength;
	shininess = _shininess;
}


LightProperties LightProperties::gold() {
    return LightProperties(
        glm::vec3(0.24725, 0.1995, 0.0745),
        glm::vec3(0.75164, 0.60648, 0.22648),
        glm::vec3(0.628281, 0.555802, 0.366065),
        128.0f * 0.4f);
}
LightProperties LightProperties::jade() {
    return LightProperties(
        glm::vec3(0.135, 0.2225, 0.1575),
        glm::vec3(0.54, 0.89, 0.63),
        glm::vec3(0.316228, 0.316228, 0.316228),
        128.0f * 0.1f);

}
LightProperties LightProperties::mettalic() {
    return LightProperties(
        glm::vec3(0.19225, 0.19225, 0.19225),
        glm::vec3(0.50754, 0.50754, 0.50754),
        glm::vec3(0.508273, 0.508273, 0.508273),
        128.0f * 0.5f);
}
LightProperties LightProperties::standard() {
    return LightProperties(
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        128.0f);
}


