#pragma once
#include <string>

namespace rlgl{

class Material
{
public:
	virtual void initialize(const std::string& texturePath);

protected:
	GLuint ID;
};

}

