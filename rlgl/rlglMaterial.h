#pragma once
#include <string>
#include <glad/gl.h>

namespace rlgl{

class Material
{
public:
	virtual void initialize(const std::string& texturePath, bool repeatTexture);
	GLuint glID = 0;

protected:
};

}

