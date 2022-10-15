#include "rlglRenderer.h"
#include "rlglObject.h"
#include "rlglScene.h"
#include "rlglMesh.h"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


using namespace rlgl;

Renderer::Renderer() {}

void Renderer::render(Scene& scene, const Camera& cam) {
	
	rlgl::cObjectIt it = scene.cbeginObject();

	glm::mat4 PVmat = cam.projectionMatrix() * cam.viewMatrix();

	for (rlgl::cObjectIt it = scene.cbeginObject(); it != scene.cendObject(); it++) {
		render(scene, PVmat, *it);
	}

}


void Renderer::render(const Scene& scene, const glm::mat4& projViewMat, Object* obj) {

	const Shader*	currentShader	= scene.shader(obj->shaderID);
	const Material* currentMaterial = scene.material(obj->materialID);
	const Mesh*		currentMesh		= scene.mesh(obj->meshID);
	if (!currentMesh) {
		throw("Renderer::render - Mesh does not exist");
	}
	if (!currentShader) {
		throw("Renderer::render - Shader does not exist");
	}

	if(currentShader->glID != lastUsedShaderID){
		currentShader->use();
	}
	if(obj->hasColor()){
		currentShader->setVec4("color", obj->getColor());
	}
	currentShader->setBool("highlight", obj->hasHighlight());
	currentShader->setMat4x4("projView", projViewMat);
	currentShader->setMat4x4("model", obj->modelMatrix());

	if (currentMaterial) {
		if(currentMaterial->glID != lastUsedMaterialID){
			glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
			glBindTexture(GL_TEXTURE_2D, currentMaterial->glID);
			//currentShader->setInt("textureID", currentMaterial->glID);
		}
	}


	if(currentMesh->VAO != lastUsedMeshID){
		currentMesh->bind();
	}
	currentMesh->draw();


	lastUsedShaderID = currentShader->glID;
	lastUsedMeshID   = currentMesh->VAO;
	if(currentMaterial){
		lastUsedMaterialID = currentMaterial->glID;
	}
}
