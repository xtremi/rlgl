#pragma once
#include <glm/glm.hpp>
#include "rlGeometry.h"

namespace rlgl{

	class Object;
	class Scene;
	class Camera;
	class Frustum;

	bool isInFrustum(const Frustum& frustum, const rl::BoundingBox& bbox);

class Renderer{
public:
	Renderer();
	void render(Scene& scene, const Camera& cam);

protected:
	void render(const Scene& scene, const glm::mat4& projViewMat, Object* obj);
	uint32_t lastUsedShaderID = UINT32_MAX;
	uint32_t lastUsedMaterialID = UINT32_MAX;
	uint32_t lastUsedMeshID = UINT32_MAX;
};


}

