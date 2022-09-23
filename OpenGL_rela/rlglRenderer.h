#pragma once

namespace rlgl{

	class Object;
	class Scene;

class Renderer
{
public:
	void render(const Scene& scene);

protected:
	void render(const Object* obj);
};


}

