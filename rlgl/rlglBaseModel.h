#pragma once
#include <glm/glm.hpp>
#include "rlglObject.h"

namespace rlgl {

	class Scene;

	class BaseModel
	{
		friend Scene;

	public:
		BaseModel() = default;

	protected:
		std::vector<Object*> objects;
	};

	/*!
		3D Coordinate sytem
	*/
	class CSYSmodel : public BaseModel {
	public:
		CSYSmodel(
			float axesLength = 5.0f,
			float axesWidth = 0.2f,
			const glm::vec3& position = glm::vec3(0.0f));

		//void create();
		void setColors(
			const glm::vec3& xAxis,
			const glm::vec3& yAxis,
			const glm::vec3& zAxis);

		void setPosition(const glm::vec3& position);
		void setSize(float axesLength, float axesWidth);

	protected:
		float _axesLength;
		float _axesWidth;
		glm::vec3 _position;
	};

	/*!
		Aim cross (UI)
	*/
	class AimCross : public BaseModel {
	public:
		AimCross(
			const glm::vec3& color,
			float length = 0.2f,
			float width = 0.01f);
	};

}