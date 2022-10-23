#pragma once
#include <glm/glm.hpp>
#include "rlGeometry.h"

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
	const glm::mat4& modelMatrix();
	void setModelMatrix(const glm::mat4& mat);
	void setPosition(const glm::vec3& pos);
	void translate(const glm::vec3& transl);
	void setRotation(float angle, const glm::vec3& rotAxis);
	void rotate(float angle, const glm::vec3& rotAxis);
	void setScale(float scale);
	void scale(float scale);
	void setScale(const glm::vec3& scale);
	void scale(const glm::vec3& scale);
	void calculateModelMatrix();
	bool isInView();
	void setInViewState(bool state);

	glm::vec3 getPosition() const  { return position; }


public:
	uint64_t meshID;
	uint64_t shaderID;
	uint64_t materialID;	

protected:
	bool _isInView		   = true;
	bool _hasColor         = false;
	bool _hasHighlight     = false;
	//rl::BoundingBox	bbox;
	glm::vec4 color		   = glm::vec4(1.f);
	glm::vec3 scaleVec	   = glm::vec3(1.f);
	glm::vec3 position	   = glm::vec3(0.f);
	glm::vec3 rotAxis	   = glm::vec3(1.f, 0.f, 0.f);
	glm::mat4 _modelMatrix = glm::mat4(1.f);
	float rotAngle         = 0.f;
	
	bool needMatrixRecalculation = true;
};



}