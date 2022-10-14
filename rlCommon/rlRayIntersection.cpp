#include "rlRayIntersection.h"

//https://gamedev.stackexchange.com/a/18459/84391
bool rl::rayIntersection(const Ray& ray, const BoundingBox& bbox) {

	float t1 = (bbox.minC.x - ray.origin().x) * ray.directionInv().x;
	float t2 = (bbox.maxC.x - ray.origin().x) * ray.directionInv().x;
	float t3 = (bbox.minC.y - ray.origin().y) * ray.directionInv().y;
	float t4 = (bbox.maxC.y - ray.origin().y) * ray.directionInv().y;
	float t5 = (bbox.minC.z - ray.origin().z) * ray.directionInv().z;
	float t6 = (bbox.maxC.z - ray.origin().z) * ray.directionInv().z;

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	float t;
	// if tmax < 0, ray (line) is intersecting box, but the whole box is behind us
	if (tmax < 0.f){
		t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect box
	if (tmin > tmax){
		t = tmax;
		return false;
	}

	t = tmin;
	return true;
}