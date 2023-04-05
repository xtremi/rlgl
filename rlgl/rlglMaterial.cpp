#include "rlglMaterial.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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


TexturedMaterial::TexturedMaterial(const std::string& texturePath, bool repeatTexture) {
	defineTexture(texturePath, repeatTexture);
}

void TexturedMaterial::defineTexture(const std::string& texturePath, bool repeatTexture) {

	int width, height, nrChannels;
	unsigned char* imgData = stbi_load(
		texturePath.c_str(), &width, &height, &nrChannels, 0);

	glGenTextures(1, &glID);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);

	if(repeatTexture){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imgData);
}

