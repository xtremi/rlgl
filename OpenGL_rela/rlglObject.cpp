#include "rlglObject.h"

using namespace rlgl;

Object::Object(uint64_t _meshID, uint64_t _shaderID, uint64_t _materialID) :
	meshID{ _meshID }, shaderID{ _shaderID }, materialID{_materialID}
{

}