#pragma once
#include "rlOctStruct.h"
#include <vector>
#include <map>
#include <set>

namespace rl{


class OctStructTreeItem {
public:
	OctStructTreeItem(const std::string& _address, void* object = nullptr) : address{ _address } {
		if (object) objects.insert(object);
	}
	std::set<void*> objects;
	std::string		address;
	std::map<std::string, OctStructTreeItem*> children;

	OctStructTreeItem* insertObject(void* object, const std::string& addr);
};



class OctStructTree {
public:
	OctStructTree() {
		root = new OctStructTreeItem("0", nullptr);
	}
	std::map<void*, std::string> octStructAddressMap;
	OctStruct octStruct;
	OctStructTreeItem* root;


	void addObject(void* obj, const OctCoord& bboxMin, const OctCoord& bboxMax);
	void removeObject(void* obj);

	std::string getBoundingBoxAddress(const OctCoord& bboxMin, const OctCoord& bboxMax);
};





}