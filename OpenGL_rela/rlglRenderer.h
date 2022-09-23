#pragma once
#include <glm/glm.hpp>

namespace rlgl{

	class Object;
	class Scene;

class Renderer{
public:
	Renderer(float aspect = 800.f / 600.f) : aspectRatio{aspect}{}
	void render(const Scene& scene);

protected:
	void render(const Scene& scene, const Object* obj);

	float aspectRatio;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
};


}

