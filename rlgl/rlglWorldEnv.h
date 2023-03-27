#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace rlgl {

	class StandardLight{
	public:
		glm::vec3 pos;
		float     intensity;
	};

	class WorldEnv {
	public:
		std::vector<StandardLight> lights;
	};

}