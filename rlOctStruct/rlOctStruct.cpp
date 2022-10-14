#include "rlOctStruct.h"
using namespace rl;

/*!
*                |-| half size level 2
x------------------x
|         |    |-|-|
|         |--------|
|		  |    |   |
x---------x--------x ---> x
|         |        |
|	      |		   |
x------------------x
  size        |----| halfSize level 1          
|---------| 

*/
float OctStruct::halfSizeAtLevel(int level) {
	return size / ((float)level * 2.f);
}

void OctStruct::setXYZhalfSize(int level, bool posX, bool posY, bool posZ, glm::vec3& c) {
	float hs = halfSizeAtLevel(level);
	c.x = posX ? hs : -hs;
	c.y = posY ? hs : -hs;
	c.z = posZ ? hs : -hs;
}

//Local center at a specific level (center is center of cube a addr in level)
void OctStruct::localLevelCenter(char addr, int level, glm::vec3& c) {
	switch (addr)
	{
	case '1': setXYZhalfSize(level, true, true, true,	 c); break;
	case '2': setXYZhalfSize(level, false, true, true,	 c); break;
	case '3': setXYZhalfSize(level, false, false, true,  c); break;
	case '4': setXYZhalfSize(level, true, false, true,	 c); break;
	case '5': setXYZhalfSize(level, true, true, false,	 c); break;
	case '6': setXYZhalfSize(level, false, true, false,  c); break;
	case '7': setXYZhalfSize(level, false, false, false, c); break;
	case '8': setXYZhalfSize(level, true, false, false,  c); break;
	default:
		throw("rl::OctStruct - Invalid address");
		break;
	}
}

//Local center of address (center is at 0., 0., 0.)
void OctStruct::localCenter(const std::string& addr, glm::vec3& coord) {
	if (addr.size() == 0) {
		return;
	}
	
	glm::vec3 lc;
	coord = glm::vec3(0.f);
	int level = 1;
	for (char c : addr) {
		localLevelCenter(c, level++, lc);
		coord += lc;
	}

}

void OctStruct::localBoundingBox(const std::string& addr, rl::BoundingBox& bbox) {
	//Special case for root (for now):
	if (addr == "0") {
		bbox = BoundingBox::createCubeBoundingBox(glm::vec3(0.f), size * 2.f);
		return;
	}
	
	int level = addr.size();
	glm::vec3 center;
	localCenter(addr, center);

	float bbWidth = size / glm::pow(2.f, (level - 1));
	bbox = BoundingBox::createCubeBoundingBox(center, bbWidth);
}

std::string OctStruct::getOctAddress(const glm::vec3& coord) {
	std::string addr = "";
	glm::vec3 lc, c;
	c = coord - center;

	for (int level = 1; level < (depth + 1); level++) {
		char cell = '0';
		if      (c.x >= 0.f && c.y >= 0.f && c.z >= 0.f) cell = '1';
		else if (c.x <  0.f && c.y >= 0.f && c.z >= 0.f) cell = '2';
		else if (c.x <  0.f && c.y <  0.f && c.z >= 0.f) cell = '3';
		else if (c.x >= 0.f && c.y <  0.f && c.z >= 0.f) cell = '4';
		else if (c.x >= 0.f && c.y >= 0.f && c.z <  0.f) cell = '5';
		else if (c.x <  0.f && c.y >= 0.f && c.z <  0.f) cell = '6';
		else if (c.x <  0.f && c.y <  0.f && c.z <  0.f) cell = '7';
		else if (c.x >= 0.f && c.y <  0.f && c.z <  0.f) cell = '8';
		addr.push_back(cell);
		localLevelCenter(cell, level, lc);
		c -= lc;
	}
	return addr;
}



std::string rl::getCommonAddress(const std::string& addr1, const std::string& addr2) {
	std::string commonAddr;
	int i = 0;
	while (addr1[i] == addr2[i]) {
		commonAddr += addr2[i++];
		if (i == addr2.size()) break;
	}
	return commonAddr;
}