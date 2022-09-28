#include <iostream>
#include <vector>
#include <string>

#include "rlOctStructTree.h"



int main()
{
	rl::OctStruct octStruct({ 0.f, 0.f, 0.f }, 100.f, 5);

	std::vector<std::string> addresses({
		"123", "223", "323", "823"
	});

	rl::OctCoord localCenter;
	for (std::string addr : addresses) {
		octStruct.localCenter(addr, localCenter);
		std::cout << addr << ": " << localCenter.x << ", " << localCenter.y << ", " << localCenter.z << std::endl;
	}

	std::vector<rl::OctCoord> coords({
		{20., 20., 20.},
		{-10., -50., -90.},
		{30., 3., 3.}
	});

	std::string addr;
	for (rl::OctCoord c : coords) {
		addr = octStruct.getOctAddress(c);
		std::cout << c.x << ", " << c.y << ", " << c.z << ": " << addr << std::endl;
	}

	float data1[2] = { 99.f, 99.f };
	float data2[2] = { 99.f, 99.f };
	float data3[2] = { 99.f, 99.f };

	rl::OctStructTree octTree;
	octTree.octStruct = octStruct;
	octTree.addObject((void*)data1, { 90.f, 90.f, 90.f }, { 92.f, 92.f, 99.f });
	octTree.addObject((void*)data1, { 90.f, 90.f, 90.f }, { 92.f, 92.f, 99.f });



}

