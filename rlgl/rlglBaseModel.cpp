#include "rlglBaseModel.h"
#include "rlglShaderBank.h"
#include "rlglColors.h"
#include "rlglMeshBank.h"
using namespace rlgl;

AimCross::AimCross(
	const glm::vec3& color,
	float length,
	float width)
{
	for (int i = 0; i < 2; i++) {
		objects.push_back(new rlgl::Object(
			rlgl::MeshBank::defaultSquare(),
			rlgl::ShaderBank::standardUIcolShader()));
		objects[i]->setColor(color);
		objects[i]->setPosition(glm::vec3(0.f, 0.f, 0.2f));
	}

	objects[0]->setScale(glm::vec3(length, width, 1.f));
	objects[1]->setScale(glm::vec3(width, length, 1.f));
}

CSYSmodel::CSYSmodel(
	float axesLength,
	float axesWidth,
	const glm::vec3& position)
{
	for (int i = 0; i < 3; i++) {
		objects.push_back(new rlgl::Object(
			rlgl::MeshBank::defaultCube(),
			rlgl::ShaderBank::standardColorShader()));
	}

	setColors(rlgl::color::red, rlgl::color::blue, rlgl::color::green);
	_position = position;
	setSize(axesLength, axesWidth); //also sets position
}

void CSYSmodel::setColors(
	const glm::vec3& xAxis,
	const glm::vec3& yAxis,
	const glm::vec3& zAxis)
{
	objects[0]->setColor(xAxis);
	objects[1]->setColor(yAxis);
	objects[2]->setColor(zAxis);
}

void CSYSmodel::setPosition(const glm::vec3& position)
{
	_position = position;

	static const std::vector<glm::vec3> axesDir({
		glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0.f , 0.f, 1.f)
		});

	for (int i = 0; i < axesDir.size(); i++) {
		objects[i]->setPosition(_position + axesDir[i] * _axesLength / 2.f);
	}
}


void CSYSmodel::setSize(float axesLength, float axesWidth)
{
	_axesLength = axesLength;
	_axesWidth = axesWidth;

	std::vector<glm::vec3> axesScales({
		glm::vec3(_axesLength, _axesWidth, _axesWidth),
		glm::vec3(_axesWidth, _axesLength, _axesWidth),
		glm::vec3(_axesWidth, _axesWidth, _axesLength)
		});

	for (int i = 0; i < 3; i++) {
		objects[i]->setScale(axesScales[i]);
	}
	setPosition(_position);	//size affects position
}



