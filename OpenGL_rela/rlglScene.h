#pragma once
#include <vector>

//#include "rlglShader.h"
//#include "rlglMaterial.h"

namespace rlgl{

	class Mesh;
	class Object;
	class Shader;
	class Renderer;
	class Material;

class Scene
{
public:
	uint64_t addMesh(const Mesh* mesh) { meshes.push_back(mesh); return meshes.size() - 1; }
	uint64_t addShader(Shader shader) { shaders.push_back(shader); return shaders.size() - 1; }
	uint64_t addMaterial(Material material) { materials.push_back(material); return materials.size() - 1; }

	uint64_t addObject(Object* obj) { objects.push_back(obj); return objects.size() - 1; }

protected:
	std::vector<const Mesh*> meshes;
	std::vector<Shader> shaders;
	std::vector<Material> materials;
	std::vector<Object*> objects;
	
};


}
