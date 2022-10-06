#pragma once
#include "rlOctStruct.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <set>

namespace rl{

	class OctStructObject {
	public:
		void* data = nullptr;
		OctCoord bboxMin, bboxMax;

		bool operator<(const OctStructObject& rhs) const {
			return data < rhs.data;
		}
		bool operator==(const OctStructObject& rhs) const {
			return data == rhs.data;
		}

	};

class OctStructTreeItem {
public:
	OctStructTreeItem(
		OctStructTreeItem* _parent,
		const std::string& _address, 
		const OctStructObject* object = nullptr) : address{ _address }, parent{_parent}
	{
		if (object) objects.insert(*object);
		if (parent) parent->children[address] = this;

	}
	std::set<OctStructObject> objects;
	std::string				  address;
	OctStructTreeItem*		  parent;
	std::unordered_map<std::string, OctStructTreeItem*> children;

	OctStructTreeItem* insertObject(const OctStructObject& object, const std::string& addr);

	std::string toStr(std::string& str, int& level);
	std::string toStr();

};



class OctStructTree {
public:
	OctStructTree() {
		root = new OctStructTreeItem(nullptr, "0", nullptr);
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