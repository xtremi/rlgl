#pragma once
#include <vector>

#include "rlglShader.h"
#include "rlglMaterial.h"
#include "rlglCamera.h"

namespace rlgl{

	class Mesh;
	class Object;
	class Renderer;

	typedef std::vector<Object*>::const_iterator cObjectIt;

class Scene
{
public:
	uint64_t addMesh(const Mesh* mesh) { meshes.push_back(mesh); return meshes.size() - 1; }
	uint64_t addShader(Shader shader) { shaders.push_back(shader); return shaders.size() - 1; }
	uint64_t addMaterial(Material material) { materials.push_back(material); return materials.size() - 1; }
	uint64_t addObject(Object* obj) { objects.push_back(obj); return objects.size() - 1; }

	const Shader*   shader(uint64_t id) const { return &shaders[id]; }
	const Mesh*     mesh(uint64_t id) const { return meshes[id]; }
	const Material* material(uint64_t id) const { 
		if(id < materials.size()){
			return &materials[id];
		}
		return nullptr;
	}
	const Object*   object(uint64_t id) const { return objects[id]; }

	Object* object(uint64_t id) { return objects[id]; }

	cObjectIt cbeginObject() const { return objects.cbegin(); }
	cObjectIt cendObject() const { return objects.cend(); }

	void cleanUp();

protected:
	std::vector<const Mesh*> meshes;
	std::vector<Shader> shaders;
	std::vector<Material> materials;
	std::vector<Object*> objects;
	
	std::vector<Object*>::const_iterator objectIterator;
};


}
