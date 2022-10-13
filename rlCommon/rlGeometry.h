#include "glm/glm.hpp"

namespace rl {

	class BoundingBox {
	public:
		glm::vec3 minC, maxC;
	};

	class Ray {
	public:		
		glm::vec3 dir;
		glm::vec3 dirN;
		glm::ivec3 sign;
	};



}