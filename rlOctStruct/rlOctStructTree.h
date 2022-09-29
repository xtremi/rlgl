#pragma once
#include "rlOctStruct.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <set>

namespace rl{


class OctStructTreeItem {
public:
	OctStructTreeItem(
		OctStructTreeItem* _parent,
		const std::string& _address, 
		void* object = nullptr) : address{ _address } 
	{
		if (object) objects.insert(object);
	}
	std::set<void*>    objects;
	std::string		   address;
	OctStructTreeItem* parent;
	std::unordered_map<std::string, OctStructTreeItem*> children;

	OctStructTreeItem* insertObject(void* object, const std::string& addr);

	std::string toStr(std::string& str, int& level);
	std::string toStr();

};



class OctStructTree {
public:
	OctStructTree() {
		root = new OctStructTreeItem("0", nullptr);
	}
	//std::unordered_map<void*, std::string> octStructAddressMap;
	std::unordered_map<void*, OctStructTreeItem*> octStructTreeItemMap;
	OctStruct octStruct;
	OctStructTreeItem* root;


	void addObject(void* obj, const OctCoord& bboxMin, const OctCoord& bboxMax);
	void moveObject(void* obj, const OctCoord& bboxMin, const OctCoord& bboxMax);
	std::vector<void*> getObjects(const OctCoord& bboxMin, const OctCoord& bboxMax);
	std::vector<void*> getObjects(const std::string& address);


	void removeObject(void* obj);

	std::string getBoundingBoxAddress(const OctCoord& bboxMin, const OctCoord& bboxMax);

	std::string toStr();
};





}