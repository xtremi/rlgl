#include "rlglScene.h"
#include "rlglRenderer.h"
using namespace rlgl;

void Scene::cleanUp() {
	for (Object* obj : objects) {
		delete obj;
	}
	objects.clear();
}