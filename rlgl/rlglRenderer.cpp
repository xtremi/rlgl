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
	currentShader->setMat4x4("projection", cam.projectionMatrix());
	currentShader->setMat4x4("view",  cam.viewMatrix());
	currentShader->setMat4x4("model", obj->modelMatrix);

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
