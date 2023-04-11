#include "rlglScene.h"
#include "rlglRenderer.h"
using namespace rlgl;

void Scene::cleanUp() {
	for (Object* obj : objects) {
		delete obj;
	}
	objects.clear();
}


void Scene::addModel(BaseModel* model) {

	for (Object* obj : model->objects) {
		addObject(obj);
	}

}
