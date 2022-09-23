#include "rlglRenderer.h"
#include "rlglObject.h"
#include "rlglScene.h"
#include "rlglMesh.h"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace rlgl;


void Renderer::render(const Scene& scene) {
	
	projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(glm::vec3(-2.f, -2.f, 1.5f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f));

	rlgl::cObjectIt it = scene.cbeginObject();

	for (rlgl::cObjectIt it = scene.cbeginObject(); it != scene.cendObject(); it++) {
		render(scene, *it);
	}

}


void Renderer::render(const Scene& scene, const Object* obj) {


	const Shader* currentShader = scene.shader(obj->shaderID);
	currentShader->use();
	currentShader->setMat4x4("projection", projectionMatrix);
	currentShader->setMat4x4("view", viewMatrix);
	currentShader->setMat4x4("model", obj->modelMatrix);

	//glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	//glBindTexture(GL_TEXTURE_2D, ctxt.textures[0]);

	const Mesh* currentMesh = scene.mesh(obj->meshID);
	currentMesh->bind();
	currentMesh->draw();
	glDrawArrays(GL_TRIANGLES, 0, 36);


}
