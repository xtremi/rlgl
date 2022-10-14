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
class OctreeStruct {
public:
	OctreeStruct(){}
	OctreeStruct(const glm::vec3& _center, float _size, int _depth) 
		: center{_center}, size{_size}, depth{_depth}{}

	glm::vec3		localLevelCenter(char addr, int level) const;
	glm::vec3		addressCenter(const std::string& addr) const;
	rl::BoundingBox addressBoundingBox(const std::string& addr) const;
	std::string		getOctAddress(const glm::vec3& coord);

	glm::vec3 center;
	float size = 100.f;
	int depth = 5;
protected:
	float halfSizeAtLevel(int level) const;
	void setXYZhalfSize(int level, bool posX, bool posY, bool posZ, glm::vec3& coord) const;
};

}