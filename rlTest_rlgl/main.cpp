#include <iostream>
#include "rlglCamera.h"
#include <string>

int test_rlglCamera();

void runTest(int(*func)(), const std::string& testName);


int main(int argc, char* argv[]){

	runTest(test_rlglCamera, "rlglCamera");

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
#include <iomanip>      // std::setprecision

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

int test_rlglCamera() {

	rlgl::Camera camera;
	camera.aspectRatio = 1.5;
	camera.far  = 100.f;
	camera.near = 10.f;
	camera.fov = 45.f;
	camera.position = glm::vec3(0.f);
	camera.front = glm::vec3(1.0f, 0.f, 0.f);
	camera.upVector = glm::vec3(0.f, 0.f, 1.f);

	camera.computeFrustum_method1();
	printFrustum(camera.frustum);
	std::cout << "\n";
	camera.computeFrustum_method2();
	printFrustum(camera.frustum);
	std::cout << "\n";
	camera.computeFrustum_method3();
	printFrustum(camera.frustum);
	return 0;
}