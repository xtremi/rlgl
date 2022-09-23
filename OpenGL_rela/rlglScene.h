#pragma once
#include <vector>


namespace rlgl{

	class Mesh;
	class Object;
	class Shader;
	class Renderer;
	class Material;

class Scene
{
public:

protected:
	std::vector<Mesh> meshes;
	std::vector<Shader> shaders;
	std::vector<Object*> objects;
};


}
