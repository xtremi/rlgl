#include "rlglMath.h"
#include  <cstdlib>

float rlgl::rand(float min, float max) {
	return min + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}