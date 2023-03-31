#pragma once
#include <glm/glm.hpp>
#include "rlGeometry.h"
#include "rlglMesh.h"
#include "rlglShader.h"
#include "rlglMaterial.h"

namespace rlgl{
	extern int OBJECT_COUNT;

class Object
{
public:
	int id = 0;
public:
	Object(
		const MeshPtr mesh, 
		const ShaderPtr shader, 
		const MaterialPtr material);

	bool hasColor() const;
	void setColor(const glm::vec4& col);
	void setColor(const glm::vec3& col);
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
	glm::vec3 getScale() const  { return scaleVec; }


	int32_t nInstances() const { return _nInstances; }
	void setNinstances(int32_t n) { _nInstances = n; }

public:
	MeshPtr		mesh;
	ShaderPtr	shader;
	MaterialPtr material;

protected:
	bool _isInView		   = true;
	bool _hasColor         = false;
	bool _hasHighlight     = false;
	int32_t	_nInstances    = -1;
	glm::vec4 color		   = glm::vec4(1.f);
	glm::vec3 scaleVec	   = glm::vec3(1.f);
	glm::vec3 position	   = glm::vec3(0.f);
	glm::vec3 rotAxis	   = glm::vec3(1.f, 0.f, 0.f);
	glm::mat4 _modelMatrix = glm::mat4(1.f);
	float rotAngle         = 0.f;
	
	
	bool needMatrixRecalculation = true;

};



}