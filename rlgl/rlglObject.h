#pragma once
#include <glm/glm.hpp>

namespace rlgl{

class Object
{
public:
	Object(uint64_t meshID, uint64_t shaderID, uint64_t materialID);
	bool hasColor() const;
	void setColor(const glm::vec4& col);
	glm::vec4 getColor() const;

	bool hasHighlight() const;
	void setHighlight(bool state);

public:
	uint64_t meshID;
	uint64_t shaderID;
	uint64_t materialID;	
	glm::mat4 modelMatrix;

protected:
	bool _hasColor = false;
	bool _hasHighlight = false;
	glm::vec4 color;
};



}