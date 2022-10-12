#pragma once
#include <glm/glm.hpp>

namespace rlgl{

class Object
{
public:
	Object(uint64_t meshID, uint64_t shaderID, uint64_t materialID);
	bool hasColor() const {
		return _hasColor;
	}
	void setColor(const glm::vec4& col) {
		color = col;
		_hasColor = true;
	}
	glm::vec4 getColor() const {
		return color;
	}

public:
	uint64_t meshID;
	uint64_t shaderID;
	uint64_t materialID;	
	glm::mat4 modelMatrix;

protected:
	bool _hasColor = false;
	glm::vec4 color;
};



}