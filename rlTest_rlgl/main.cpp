#include <iostream>
#include "rlglCamera.h"
#include "rlglRenderer.h"
#include <string>
#include <vector>

int test_rlglFrustum();

void runTest(int(*func)(), const std::string& testName);


int main(int argc, char* argv[]){

	runTest(test_rlglFrustum, "rlglFrustum");

	return 0;
}

void runTest(int(*func)(), const std::string& testName) {
	if (func()) {
		std::cout << "Test " << testName << " FAILED" << std::endl;
	}
	else {
		std::cout << "Test " << testName << " PASSED" << std::endl;
	}
}

#include <vector>
#include <iomanip>  // std::setprecision

void printFrustum(const rlgl::Frustum& _frustum) {

	rlgl::Frustum frustum = _frustum;

	static const std::vector<std::string> fPlaneName({ "near", "far", "top", "bottom", "left", "right" });
	std::cout << std::fixed << std::setprecision(4);
	for (int i = 0; i < 6; i++) {
		std::cout << "Plane " << fPlaneName[i] << "\t"
			<< frustum.planes[i].normal().x << ", "
			<< frustum.planes[i].normal().y << ", "
			<< frustum.planes[i].normal().z << ", " << std::endl;
	}
}

#include <glm/gtc/epsilon.hpp>
bool frustumsAreEqual(const std::vector<rlgl::Frustum>& frustums) {

	if (frustums.size() < 2) return false;

	for (int i = 1; i < frustums.size(); i++) {

		for (int j = 0; j < 6; j++) {
			
			glm::bvec3 eq = glm::epsilonEqual(frustums[0].planes[j].normal(), frustums[i].planes[j].normal(), 1e-6f);
			if (!eq.x || !eq.y || !eq.z) {
				return false;
			}

		}
	}
	return true;
}

int test_rlglFrustum() {

	rlgl::Camera camera;
	camera.aspectRatio = 1.5;
	camera.far  = 100.f;
	camera.near = 10.f;
	camera.fov = 45.f;
	camera.position = glm::vec3(0.f);
	camera.front = glm::vec3(1.0f, 0.f, 0.f);
	camera.upVector = glm::vec3(0.f, 0.f, 1.f);

	std::vector<rlgl::Frustum> frustums;
	camera.computeFrustum_method1();
	printFrustum(camera.frustum);
	frustums.push_back(camera.frustum);
	std::cout << "\n";
	camera.computeFrustum_method2();
	printFrustum(camera.frustum);
	frustums.push_back(camera.frustum);
	std::cout << "\n";
	camera.computeFrustum_method3();
	printFrustum(camera.frustum);
	frustums.push_back(camera.frustum);

	if (!frustumsAreEqual(frustums)) return 1;

	//Should be in frustum:
	rl::BoundingBox bbox(glm::vec3(20.f, -5.f, -5.f), glm::vec3(25.f, 5.f, 5.f));
	if (!rlgl::isInFrustum(camera.frustum, bbox)) {
		return 1;
	}

	//Should not be in frustum
	bbox.translate(glm::vec3(81.f, 0.f, 0.f));
	if (rlgl::isInFrustum(camera.frustum, bbox)) {
		return 1;
	}

	//Should be in frustum
	bbox.translate(glm::vec3(-2.f, 0.f, 0.f));
	if (!rlgl::isInFrustum(camera.frustum, bbox)) {
		return 1;
	}

	return 0;
}