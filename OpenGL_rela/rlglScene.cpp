#include "rlglScene.h"
#include "rlglRenderer.h"
using namespace rlgl;



void Scene::render(Renderer* renderer) {

	for (Object* obj : objects) {
		renderer->render(obj);
	}

}
