#pragma once
#include <glm/glm.hpp>

namespace rlgl{

	class Object;
	class Scene;
	class Camera;

class Renderer{
public:
	Renderer();
	void render(const Scene& scene, const Camera& cam);

protected:
	void render(const Scene& scene, const Camera& cam, const Object* obj);
};


}

