#include "rlglRenderer.h"
#include "rlglObject.h"
#include "rlglScene.h"
#include "rlglMesh.h"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


using namespace rlgl;

Renderer::Renderer() {}

void Renderer::render(Scene& scene, const Camera& cam) 
{
	glm::mat4 PVmat = cam.pvMatrix();

	rlgl::cObjectIt it = scene.cbeginObject();
	for (rlgl::cObjectIt it = scene.cbeginObject(); it != scene.cendObject(); it++) {
		if((*it)->isInView()){
			render(scene, PVmat, *it);
		}
	}

	lastUsedShaderID = UINT32_MAX;
	lastUsedMeshID = UINT32_MAX;
	lastUsedMaterialID = UINT32_MAX;
}


void Renderer::render(
	const Scene&	 scene, 
	const glm::mat4& projViewMat, 
	Object*			 obj) 
{
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
		currentShader->setWorldUniforms(projViewMat, scene.worldEnv);
	}
	currentShader->setObjectUniforms(obj);

	if (currentMaterial) {
		if(currentMaterial->glID != lastUsedMaterialID){
			glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
			glBindTexture(GL_TEXTURE_2D, currentMaterial->glID);
		}
	}

	if(currentMesh->VAO != lastUsedMeshID){
		currentMesh->bind();
	}
	currentMesh->draw(obj->nInstances());


	lastUsedShaderID = currentShader->glID;
	lastUsedMeshID   = currentMesh->VAO;
	if(currentMaterial){
		lastUsedMaterialID = currentMaterial->glID;
	}
}



bool rlgl::isInFrustum(const Frustum& frustum, const rl::BoundingBox& bbox, bool strict) {
	static const size_t NUMBER_OF_PLANES = 6;

	std::vector<glm::vec3> bboxCorners;
	if(strict){
		bboxCorners = bbox.corners();
	}
	else{
		bboxCorners = std::vector<glm::vec3>({bbox.minC, bbox.maxC});
	}
	std::vector<int> plane_outside_of(8);
	
	size_t cornerIndex = 0;
	for (const glm::vec3& corner : bboxCorners) {

		bool cornerInFrustum = true;
		for (int i = 0; i < NUMBER_OF_PLANES; i++) {
			
			const rl::Plane* pln = &frustum.planes[i];
			glm::vec3 vec = corner - pln->position;
			float d = glm::dot(vec, pln->normal());
			if (d < 0.f) {
				cornerInFrustum = false;
				plane_outside_of[cornerIndex++] = i;
				break;
			}
		}

		if (cornerInFrustum) {
			return true;
		}
	}

	//If both corners are outside frustum, but not
	//based on the same planes, we keep it.
	//This is for large object, larger than the frusum space
	if(!strict){
		if (plane_outside_of[0] != plane_outside_of[1]) {
			return true;
		}
	}
	//for (int i = 1; i < 6; i++) {
	//	if (plane_outside_of[0] != plane_outside_of[i]) {
	//		return true;
	//	}
	//}

	return false;
}
