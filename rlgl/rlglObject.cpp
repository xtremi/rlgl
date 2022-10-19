#include "rlglObject.h"
#include "glm/gtx/quaternion.hpp"

using namespace rlgl;

Object::Object(uint64_t _meshID, uint64_t _shaderID, uint64_t _materialID) :
	meshID{ _meshID }, shaderID{ _shaderID }, materialID{_materialID}
{
	_modelMatrix = glm::mat4(1.f);
}

bool Object::hasColor() const {
	return _hasColor;
}
void Object::setColor(const glm::vec4& col) {
	color = col;
	_hasColor = true;
}
glm::vec4 Object::getColor() const {
	return color;
}
void Object::setHighlight(bool state) {
	_hasHighlight = state;
}
bool Object::hasHighlight() const {
	return _hasHighlight;
}

void Object::setPosition(const glm::vec3& pos) {
	needMatrixRecalculation = true;
	position = pos;
}
void Object::translate(const glm::vec3& transl) {
	needMatrixRecalculation = true;
	position += transl;
}
void Object::setRotation(float _angle, const glm::vec3& _rotAxis) {
	needMatrixRecalculation = true;
	rotAxis = _rotAxis;
	rotAngle = _angle;
}
void Object::rotate(float _angle, const glm::vec3& _rotAxis) {
	needMatrixRecalculation = true;
	rotAxis = _rotAxis;
	rotAngle += _angle;
}
void Object::setScale(float _scale) {
	needMatrixRecalculation = true;
	scaleVec = glm::vec3(_scale);
}
void Object::scale(float scale) {
	needMatrixRecalculation = true;
	scaleVec *= scale;
}
void Object::setScale(const glm::vec3& scale) {
	needMatrixRecalculation = true;
	scaleVec = scale;
}
void Object::scale(const glm::vec3& scale) {
	needMatrixRecalculation = true;
	scaleVec *= scale;
}
void Object::calculateModelMatrix() {
	glm::mat4 translation = glm::translate(glm::mat4(1.f), position);
	//glm::mat4 rotation = glm::toMat4(orientation); // Rotation matrix
	glm::mat4 rotation = glm::rotate(glm::mat4(1.f), rotAngle, rotAxis);
	glm::mat4 scale = glm::scale(glm::mat4(1.f), scaleVec);

	_modelMatrix = translation* rotation* scale;
	needMatrixRecalculation = false;
}

void Object::setModelMatrix(const glm::mat4& mat) {
	_modelMatrix = mat;
	needMatrixRecalculation = false;
}


const glm::mat4& Object::modelMatrix() {
	if (needMatrixRecalculation) {
		calculateModelMatrix();
	}
	return _modelMatrix;
}
bool Object::isInView() {
	return _isInView;
}
void Object::setInViewState(bool state) {
	_isInView = state;
}
