#include <iostream>
#include "rlglCamera.h"


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


int test_rlglCamera() {

	rlgl::Camera camera();

	return 0;
}