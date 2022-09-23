#pragma once
#include <string>
#include <glad/glad.h>

namespace rlgl{

class Material
{
public:
	virtual void initialize(const std::string& texturePath);

protected:
	GLuint glID = 0;
};

}

