#include <iostream>
#include <vector>
#include <string>

#include "rlOctStructTree.h"

namespace rl{
float rand(float min, float max) {
	return min + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}
}

int main()
{
	rl::OctStruct octStruct(glm::vec3(0.f), 100.f, 5);

	std::vector<std::string> addresses({
		"123", "223", "323", "823"
	});

	glm::vec3 localCenter;
	for (std::string addr : addresses) {
		octStruct.localCenter(addr, localCenter);
		std::cout << addr << ": " << localCenter.x << ", " << localCenter.y << ", " << localCenter.z << std::endl;
	}

	std::vector<glm::vec3> coords({
		{20., 20., 20.},
		{-10., -50., -90.},
		{30., 3., 3.}
	});

	std::string addr;
	for (glm::vec3 c : coords) {
		addr = octStruct.getOctAddress(c);
		std::cout << c.x << ", " << c.y << ", " << c.z << ": " << addr << std::endl;
	}

	float data[1000] = { 99.f, 99.f };

	rl::OctStructTree octTree(octStruct);
	octTree.addObject((void*)&data[1], { { 90.f, 90.f, 90.f }, { 92.f, 92.f, 92.f } });	  //111
	octTree.addObject((void*)&data[2], {{ 91.f, 91.f, 91.f }, { 93.f, 93.f, 93.f } });	  //111
	octTree.addObject((void*)&data[3], {{ 71.2f, 71.2f, 71.2f }, { 73.f, 73.f, 73.f } }); //11711
	octTree.addObject((void*)&data[4], {{ 91.f, 91.f, 50.f }, { 100.f, 100.f, 51.f } });  //115
	octTree.addObject((void*)&data[5], {{ -91.f, -91.f, -91.f }, { -93.f, -93.f, -93.f } }); //777
	octTree.addObject((void*)&data[6], {{ -51.f, -91.f, -91.f }, { -93.f, -93.f, -93.f } }); //77
	octTree.addObject((void*)&data[7], {{ 1.f, 1.f, 1.f }, { 2.f, 2.f, 2.f } });			//17771
	std::cout << "Should be 7 objects totally" << std::endl;
	std::cout << octTree.toStr() << "\n";

	octTree.removeObject((void*)&data[4]);
	octTree.removeObject((void*)&data[2]);
	octTree.removeObject((void*)&data[3]);
	octTree.removeObject((void*)&data[7]);
	octTree.removeObject((void*)&data[6]);
	octTree.removeObject((void*)&data[1]);
	octTree.removeObject((void*)&data[5]);
	std::cout << "Should be no children" << std::endl;
	std::cout << octTree.toStr() << "\n";


	int maxPoints = 1e2;
	float* tempdata = new float[maxPoints];
	rl::BoundingBox bbox;
	std::vector<void*> allAddedObjects;
	for (int i = 0; i < maxPoints; i++) {
		bbox.minC.x = rl::rand(-100.f, 100.f);
		bbox.minC.y = rl::rand(-100.f, 100.f);
		bbox.minC.z = rl::rand(-100.f, 100.f);
		bbox.maxC.x = bbox.minC.x + 0.5f;
		bbox.maxC.y = bbox.minC.y + 0.5f;
		bbox.maxC.z = bbox.minC.z + 0.5f; 

		octTree.addObject((void*)&tempdata[i], bbox); //17771
		allAddedObjects.push_back((void*)&tempdata[i]);
	}
	for (void* obj : allAddedObjects) {
		octTree.removeObject(obj);
	}

	std::cout << "Should be no children" << std::endl;
	std::cout << octTree.toStr() << "\n";
	

	delete[] tempdata;
}

