#pragma once
#include "glm/glm.hpp"

namespace rl {

	class Cube {
	public:
		Cube(const glm::vec3& minCoords, const glm::vec3& maxCoords);
		Cube() {}
		glm::vec3 minC = glm::vec3(-1.f), maxC = glm::vec3(1.f);

		glm::vec3 center() const;
		glm::vec3 size() const;
		void translate(const glm::vec3& translation);

	};

	class BoundingShape {

	};
	
	class BoundingBox : public BoundingShape, public Cube {
	public:
		BoundingBox(const glm::vec3& minCoords, const glm::vec3& maxCoords);
		BoundingBox() {}

		static BoundingBox createCubeBoundingBox(const glm::vec3& center, float width);
	};


	class Plane {
	public:
		Plane(){}
		Plane(const glm::vec3& norm, const glm::vec3& pos);

		glm::vec3 normal();
		void setNormal(const glm::vec3& norm);
		glm::vec3 position = glm::vec3(0.f);
	
	protected:
		glm::vec3 _normal = glm::vec3(0.f, 0.f, 1.f);
	};


	class Ray {
	public:
		Ray(
			const glm::vec3& direction = glm::vec3(1.f, 0.f, 0.f),
			const glm::vec3& origin = glm::vec3(0.f));
		
		void setDirection(const glm::vec3& direction);
		void setOrigin(const glm::vec3& origin);

		const glm::vec3& origin() const { return _origin; }
		const glm::vec3& direction() const { return _direction; }
		const glm::vec3& directionInv() const { return _dir_inv; }

	private:		
		glm::vec3  _origin;
		glm::vec3  _direction;
		glm::vec3  _dir_inv;
	};



}