#pragma once
#include <vector>

#include "rlglShader.h"
#include "rlglMaterial.h"
#include "rlglCamera.h"
#include "rlglWorldEnv.h"

namespace rlgl{

	class Mesh;
	class Object;
	class Renderer;

	typedef std::vector<Object*>::const_iterator cObjectIt;

class Scene
{
public:
	WorldEnv worldEnv;

	uint64_t addObject(Object* obj) { objects.push_back(obj); return objects.size() - 1; }

	const Object*   object(uint64_t id) const { return objects[id]; }

	Object* object(uint64_t id) { return objects[id]; }

	cObjectIt cbeginObject() const { return objects.cbegin(); }
	cObjectIt cendObject() const { return objects.cend(); }

	void cleanUp();

protected:
	std::vector<Object*> objects;
	
	std::vector<Object*>::const_iterator objectIterator;
};


}
