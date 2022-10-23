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
		if((*it)->isInView()){
			render(scene, PVmat, *it);
		}
	}

}


void Renderer::render(
	const Scene&	 scene, 
	const glm::mat4& projViewMat, 
	Object*			 obj) {

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



bool rlgl::isInFrustum(const Frustum& frustum, const rl::BoundingBox& bbox) {

	//std::vector<glm::vec3> bboxCorners = bbox.corners();
	std::vector<glm::vec3> bboxCorners({bbox.minC, bbox.maxC});

	for (const glm::vec3& corner : bboxCorners) {

		bool cornerInFrustum = true;
		for (int i = 0; i < 6; i++) {
			const rl::Plane* pln = &frustum.planes[i];
			glm::vec3 v = corner - pln->position;
			float d = glm::dot(v, pln->normal());
			if (d < 0.f) {
				cornerInFrustum = false;
				break;
			}
		}

		if (cornerInFrustum) {
			return true;
		}
	}
	return false;
}
