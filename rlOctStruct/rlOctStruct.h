#pragma once
#include <string>
#include "rlGeometry.h"


namespace rl{

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
	OctStruct(const glm::vec3& _center, float _size, int _depth) 
		: center{_center}, size{_size}, depth{_depth}{}

	void localLevelCenter(char addr, int level, glm::vec3& coord) const;				  //Local center at a specific level (center is center of cube a addr in level)
	void localCenter(const std::string& addr, glm::vec3& coord) const;			  //Local center of address (center is at 0., 0., 0.)
	void localBoundingBox(const std::string& addr, rl::BoundingBox& coord) const;
	std::string getOctAddress(const glm::vec3& coord);

	glm::vec3 center;
	float size = 100.f;
	int depth = 5;
protected:
	float halfSizeAtLevel(int level) const;
	void setXYZhalfSize(int level, bool posX, bool posY, bool posZ, glm::vec3& coord) const;
};

}