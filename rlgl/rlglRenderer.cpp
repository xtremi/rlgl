#include "rlglRenderer.h"
#include "rlglObject.h"
#include "rlglScene.h"
#include "rlglMesh.h"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


using namespace rlgl;

Renderer::Renderer() {}

void Renderer::render(const Scene& scene, const Camera& cam) {
	
	rlgl::cObjectIt it = scene.cbeginObject();

	for (rlgl::cObjectIt it = scene.cbeginObject(); it != scene.cendObject(); it++) {
		render(scene, cam, *it);
	}

}


void Renderer::render(const Scene& scene, const Camera& cam, const Object* obj) {


	const Shader* currentShader = scene.shader(obj->shaderID);
	const Material* currentMaterial =scene.material(obj->materialID);

	currentShader->use();
	currentShader->setMat4x4("projection", cam.projectionMatrix());
	currentShader->setMat4x4("view", cam.viewMatrix());
	currentShader->setMat4x4("model", obj->modelMatrix);

	if(currentMaterial){
		glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, currentMaterial->glID);
		//currentShader->setInt("textureID", currentMaterial->glID);
	}


	const Mesh* currentMesh = scene.mesh(obj->meshID);
	currentMesh->bind();
	currentMesh->draw();

}
