#include "rlGeometry.h"
using namespace rl;

Cube::Cube(const glm::vec3& minCoords, const glm::vec3& maxCoords) 
	: minC{minCoords}, maxC{maxCoords}{}

glm::vec3 Cube::size() const {
	return maxC - minC;
}

glm::vec3 Cube::center() const {
	//a + (b - a)/2 = a + b/2 - a/2 = a/2 + b/2 = (a+b)/2 
	return (minC + maxC) / 2.f;
}

void Cube::translate(const glm::vec3& translation) {
	minC += translation;
	maxC += translation;
}

/*!
           8--------7 maxC
         /	      / |  _ y
	   /        /	|  /|
	 5--------6		| /
	 | 		  |     3
	 | 		  |   /
	 | 		  | /
minC 1--------2      --->x

Returns the the corners of the bounding box.
The bounding box is an AABB, so all axis are aligned with global XYZ axes.
*/
std::vector<glm::vec3> BoundingBox::corners() const {
	std::vector<glm::vec3> coords(8);

	glm::vec3 bsize = size();

	size_t i = 0;
	coords[i++] = minC;
	coords[i++] = minC + glm::vec3(bsize.x, 0.f, 0.f);
	coords[i++] = minC + glm::vec3(bsize.x, bsize.y, 0.f);
	coords[i++] = minC + glm::vec3(0.f, bsize.y, 0.f);

	coords[i++] = minC + glm::vec3(0.f, 0.f, bsize.z);
	coords[i++] = minC + glm::vec3(bsize.x, 0.f, bsize.z);
	coords[i++] = maxC;
	coords[i++] = maxC + glm::vec3(0.f, bsize.y, bsize.z);
	return coords;
}


/*************************************************/


BoundingBox::BoundingBox(const glm::vec3& minCoords, const glm::vec3& maxCoords) :
	Cube(minCoords, maxCoords){}


BoundingBox BoundingBox::createCubeBoundingBox(const glm::vec3& center, float width) {
	return BoundingBox(center - glm::vec3(width / 2.f), center + glm::vec3(width / 2.f));
}

/**************************************************/

Plane::Plane(const glm::vec3& norm, const glm::vec3& pos) {
	position = pos;
	setNormal(norm);
}

glm::vec3 Plane::normal() const {
	return _normal;
}
void Plane::setNormal(const glm::vec3& norm) {
	_normal = glm::normalize(norm);
}

/**************************************************/


Ray::Ray(
	const glm::vec3& direction,
	const glm::vec3& origin) :
	_origin{ origin }
{
	setDirection(direction);
}

void Ray::setDirection(const glm::vec3& direction) {
	_direction = glm::normalize(direction);
	
	glm::vec3 dirNonNull = _direction;
	for (int i = 0; i < 3; i++) {
		if (dirNonNull[i] == 0.f || dirNonNull[i] == -0.f) {
			dirNonNull[i] = FLT_MIN;
		}
	}

	_dir_inv = glm::vec3(
		1.f / dirNonNull.x,
		1.f / dirNonNull.y,
		1.f / dirNonNull.z);
}
void Ray::setOrigin(const glm::vec3& origin) {
	_origin = origin;
}