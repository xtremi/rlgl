#pragma once
#include <glm/glm.hpp>
#include "rlGeometry.h"
#include "rlglMesh.h"
#include "rlglShader.h"
#include "rlglMaterial.h"

namespace rlgl{

	class Object;
	class Scene;
	class Camera;
	class Frustum;

	bool isInFrustum(const Frustum& frustum, const rl::BoundingBox& bbox, bool strict = false);

class Renderer{
public:
	Renderer();
	void render(Scene& scene, const Camera& cam);

protected:
	void render(
		const Scene&	 scene, 
		const glm::mat4& projViewMat, 
		const glm::vec3& camPos,
		Object*			 obj);

	MeshPtr lastUsedMesh = nullptr;
	ShaderPtr lastUsedShader = nullptr;
	MaterialPtr lastUsedMaterial = nullptr;
};


}

