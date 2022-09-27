#pragma once
#include <string>
/*

		 ^ y   (z-out of screen)
		 |
x-----------------x
|   2/6  |  1/5   |
|		 | 		  |
x--------x--------x ---> x
|   3/7  |  4/8   |
|	     |		  |
x-----------------x
  size      size
|--------|--------|

*/
class OctStruct {
public:
	float centerX = 0.f, centerY = 0.f;
	float size = 100.f;
	int depth = 5;

	float halfSize(int level);
	void setXYZhalfSize(int level, bool posX, bool posY, bool posZ, float& x, float& y, float& z);

	//Local center at a specific level (center is center of cube a addr in level)
	void localLevelCenter(char addr, int level, float& x, float& y, float& z);

	//Local center of address (center is at 0., 0., 0.)
	void localCenter(const std::string& addr, float& x, float& y, float& z);


	std::string getOctAddress(float x, float y, float z);
};
