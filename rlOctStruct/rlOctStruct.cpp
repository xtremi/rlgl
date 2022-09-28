#include "rlOctStruct.h"
using namespace rl;


float OctStruct::halfSize(int level) {
	return size / ((float)level * 2.f);
}

void OctStruct::setXYZhalfSize(int level, bool posX, bool posY, bool posZ, OctCoord& c) {
	float hs = halfSize(level);
	c.x = posX ? hs : -hs;
	c.y = posY ? hs : -hs;
	c.z = posZ ? hs : -hs;
}

//Local center at a specific level (center is center of cube a addr in level)
void OctStruct::localLevelCenter(char addr, int level, OctCoord& c) {
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
void OctStruct::localCenter(const std::string& addr, OctCoord& coord) {
	if (addr.size() == 0) {
		return;
	}
	
	OctCoord lc;
	coord = OctCoord();
	int level = 1;
	for (char c : addr) {
		localLevelCenter(c, level++, lc);
		coord.x += lc.x;
		coord.y += lc.y;
		coord.z += lc.z;
	}

}

std::string OctStruct::getOctAddress(const OctCoord& coord) {
	std::string addr = "";
	OctCoord lc, c;
	c.x = coord.x - center.x;
	c.y = coord.y - center.y;
	c.z = coord.z - center.z;

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
		c.x -= lc.x;
		c.y -= lc.y;
		c.z -= lc.z;
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