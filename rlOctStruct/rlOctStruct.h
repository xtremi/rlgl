#pragma once
#include <string>

namespace rl{

struct OctCoord { float x = 0.f, y = 0.f, z = 0.f; };

std::string getCommonAddress(const std::string& addr1, const std::string& addr2);



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
	OctStruct(){}
	OctStruct(const OctCoord& _center, float _size, int _depth) : center{_center}, size{_size}, depth{_depth}{}

	OctCoord center;
	float size = 100.f;
	int depth = 5;

	float halfSize(int level);
	void setXYZhalfSize(int level, bool posX, bool posY, bool posZ, OctCoord& coord);

	//Local center at a specific level (center is center of cube a addr in level)
	void localLevelCenter(char addr, int level, OctCoord& coord);

	//Local center of address (center is at 0., 0., 0.)
	void localCenter(const std::string& addr, OctCoord& coord);


	std::string getOctAddress(const OctCoord& coord);
};

}