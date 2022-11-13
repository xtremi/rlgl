#include "rlOctreeStruct.h"
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
float OctreeStruct::halfSizeAtLevel(int level) const {
	return size / glm::pow(2.f, level);
}

void OctreeStruct::setXYZhalfSize(int level, bool posX, bool posY, bool posZ, glm::vec3& c) const {
	float hs = halfSizeAtLevel(level);
	c.x = posX ? hs : -hs;
	c.y = posY ? hs : -hs;
	c.z = posZ ? hs : -hs;
}

/*
	Returns coords of center of an octree item (1-8)
	where origo is its parent.

	Example:
	- addr = 1 and level = 1 returns  50.,50.,50.
	- addr = 2 and level = 1 returns -50.,50.,50.
	- addr = 2 and level = 2 returns -25.,25.,25.
*/
glm::vec3 OctreeStruct::localLevelCenter(char addr, int level) const {
	glm::vec3 c;
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
	return c;
}

/*!
	Given an address, returns the center of its octree item,
	origo is the center of the octree
*/
glm::vec3 OctreeStruct::addressCenter(const std::string& addr) const {	
	glm::vec3 coord = center;
	
	if (addr.size() == 0) {
		return coord;
	}

	glm::vec3 lc;
	
	int level = 1;
	for (char c : addr) {
		coord += localLevelCenter(c, level++);
	}
	return coord;
}

/*!
	Returns the bounding box of a given address.
	Origo is as center octree.
*/
rl::BoundingBox OctreeStruct::addressBoundingBox(const std::string& addr) const {
	rl::BoundingBox bbox;

	//Special case for root (for now):
	if (addr == "0") {
		return BoundingBox::createCubeBoundingBox(center, size * 2.f);
	}
	
	int level = addr.size();
	glm::vec3 center = addressCenter(addr);

	float bbWidth = size / glm::pow(2.f, (level - 1));
	return BoundingBox::createCubeBoundingBox(center, bbWidth);
}

/*!
	Returns the address in the octree of the given coord.
*/
std::string OctreeStruct::getOctAddress(const glm::vec3& coord) {
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
		c -= localLevelCenter(cell, level);;
	}
	return addr;
}


/*!
	Given two addresses, returns the common part of them (starting from the beginning)

	Example: addr1=11236 and addr2=11286 returns 112
*/
std::string rl::getCommonAddress(const std::string& addr1, const std::string& addr2) {
	std::string commonAddr;
	int i = 0;
	while (addr1[i] == addr2[i]) {
		commonAddr += addr2[i++];
		if (i == addr2.size()) break;
	}
	if (commonAddr.empty()) {
		commonAddr = "0";
	}
	return commonAddr;
}