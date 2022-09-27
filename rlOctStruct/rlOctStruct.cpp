#include "rlOctStruct.h"

float OctStruct::halfSize(int level) {
	return size / ((float)level * 2.f);
}

void OctStruct::setXYZhalfSize(int level, bool posX, bool posY, bool posZ, float& x, float& y, float& z) {
	float hs = halfSize(level);
	x = posX ? hs : -hs;
	y = posY ? hs : -hs;
	z = posZ ? hs : -hs;
}

//Local center at a specific level (center is center of cube a addr in level)
void OctStruct::localLevelCenter(char addr, int level, float& x, float& y, float& z) {
	switch (addr)
	{
	case '1': setXYZhalfSize(level, true, true, true, x, y, z); break;
	case '2': setXYZhalfSize(level, false, true, true, x, y, z); break;
	case '3': setXYZhalfSize(level, false, false, true, x, y, z); break;
	case '4': setXYZhalfSize(level, true, false, true, x, y, z); break;
	case '5': setXYZhalfSize(level, true, true, false, x, y, z); break;
	case '6': setXYZhalfSize(level, false, true, false, x, y, z); break;
	case '7': setXYZhalfSize(level, false, false, false, x, y, z); break;
	case '8': setXYZhalfSize(level, true, false, false, x, y, z); break;
	default:
		throw("rl::OctStruct - Invalid address");
		break;
	}
}

//Local center of address (center is at 0., 0., 0.)
void OctStruct::localCenter(const std::string& addr, float& x, float& y, float& z) {
	if (addr.size() == 0) {
		return;
	}
	x = y = z = 0.f;
	float lcX, lcY, lcZ;
	int level = 1;
	for (char c : addr) {
		localLevelCenter(c, level++, lcX, lcY, lcZ);
		x += lcX;
		y += lcY;
		z += lcZ;
	}

}

std::string OctStruct::getOctAddress(float x, float y, float z) {
	std::string addr = "";
	float lcX = 0.f, lcY = 0.f, lcZ = 0.f;

	for (int level = 1; level < depth + 1; level++) {
		char c = '0';
		if (x > lcX && y > lcY && z > lcZ) c = '1';
		else if (x < lcX && y > lcY && z > lcZ) c = '2';
		else if (x < lcX && y < lcY && z > lcZ) c = '3';
		else if (x > lcX && y < lcY && z > lcZ) c = '4';
		else if (x > lcX && y > lcY && z < lcZ) c = '5';
		else if (x < lcX && y > lcY && z < lcZ) c = '6';
		else if (x < lcX && y < lcY && z < lcZ) c = '7';
		else if (x > lcX && y < lcY && z < lcZ) c = '8';
		addr.push_back(c);
		localLevelCenter(c, level, lcX, lcY, lcZ);
		x -= lcX;
		y -= lcY;
		z -= lcZ;
	}
	return addr;
}