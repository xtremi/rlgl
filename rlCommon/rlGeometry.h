#include "glm/glm.hpp"

namespace rl {

	class BoundingBox {
	public:
		glm::vec3 minC, maxC;
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