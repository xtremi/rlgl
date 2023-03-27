#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace rlgl {

	class StandardLight{
	public:
		glm::vec3 pos;
		glm::vec3 color;
		float     ambientIntensity;
	};

	class WorldEnv {
	public:
		std::vector<StandardLight> lights;
	};

}