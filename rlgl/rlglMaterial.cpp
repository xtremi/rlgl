#include "rlglMaterial.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace rlgl;

Textured2dMaterial::Textured2dMaterial(const std::string& texturePath, bool repeatTexture){
	defineTexture(texturePath, repeatTexture);
}

void Textured2dMaterial::defineTexture(const std::string& texturePath, bool repeatTexture) {

	int width, height, nrChannels;
	unsigned char* imgData = stbi_load(
		texturePath.c_str(), &width, &height, &nrChannels, 0);

	glGenTextures(1, &glID);
	glBindTexture(GL_TEXTURE_2D, glID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
	stbi_image_free(imgData);

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

}

void Textured2dMaterial::bind() {
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, glID);
}



TexturedCubeMapMaterial::TexturedCubeMapMaterial(const std::vector<std::string>& texturePaths) {
	defineTextures(texturePaths);
}

void TexturedCubeMapMaterial::defineTextures(const std::vector<std::string>& texturePaths) {
	static const size_t N_CUBEMAP_FACES = 6;
	if (texturePaths.size() != N_CUBEMAP_FACES) {
		throw("CubeMatTexturedMaterial::defineTextures expects 6 texture paths, got " + std::to_string(texturePaths.size()));
	}

	int width, height, nrChannels;
	unsigned char* imgData = nullptr;

	glGenTextures(1, &glID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, glID);
	//stbi_set_flip_vertically_on_load(false);


	for(size_t i = 0; i < N_CUBEMAP_FACES; i++){
		imgData = stbi_load(
			texturePaths[i].c_str(), 
			&width, &height, &nrChannels, 0);

		if (imgData) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height,
				0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
			stbi_image_free(imgData);
		}
		else {
			stbi_image_free(imgData);
			throw("CubeMatTexturedMaterial::defineTextures failed to load image data from path " + texturePaths[i]);
		}
	}
	//stbi_set_flip_vertically_on_load(true);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TexturedCubeMapMaterial::bind() {
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, glID);
}