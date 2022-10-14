#include "rlglObject.h"

using namespace rlgl;

Object::Object(uint64_t _meshID, uint64_t _shaderID, uint64_t _materialID) :
	meshID{ _meshID }, shaderID{ _shaderID }, materialID{_materialID}
{
	modelMatrix = glm::mat4(1.f);
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
