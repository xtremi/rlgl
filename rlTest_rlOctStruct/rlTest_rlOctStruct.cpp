#include <iostream>
#include <vector>
#include <string>
#include "rlOctStruct.h"

struct Coords {
	float x, y, z;
};

int main()
{
	OctStruct octStruct;
	float x, y, z;

	std::vector<std::string> addresses({
		"123", "223", "323", "823"
	});

	for (std::string addr : addresses) {
		octStruct.localCenter(addr, x, y, z);
		std::cout << addr << ": " << x << ", " << y << ", " << z << std::endl;
	}

	std::vector<Coords> coords({
		{20., 20., 20.},
		{-10., -50., -90.},
		{30., 3., 3.}
	});
	std::string addr;
	for (Coords c : coords) {
		addr = octStruct.getOctAddress(c.x, c.y, c.z);
		std::cout << c.x << ", " << c.y << ", " << c.z << ": " << addr << std::endl;
	}


}

