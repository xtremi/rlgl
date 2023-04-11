#pragma once
#include <vector>

#include "rlglShader.h"
#include "rlglMaterial.h"
#include "rlglCamera.h"
#include "rlglWorldEnv.h"
#include "rlglBaseModel.h"

namespace rlgl{

	class Mesh;
	class Object;
	class Renderer;

	typedef std::vector<Object*>::const_iterator cObjectIt;

class Scene
{
public:
	WorldEnv worldEnv;

	void addObject(Object* obj) { objects.push_back(obj); }
	void addModel(const BaseModel& model);

	cObjectIt cbeginObject() const { return objects.cbegin(); }
	cObjectIt cendObject() const { return objects.cend(); }

	void cleanUp();

protected:
	std::vector<Object*> objects;
	std::vector<Object*>::const_iterator objectIterator;
};


}
