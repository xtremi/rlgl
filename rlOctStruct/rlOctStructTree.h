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
		BoundingBox bbox;

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
		const rl::BoundingBox& bbox,
		const OctStructObject* object = nullptr)
		: address{ _address }, parent{_parent}
	{
		if (object) objects.insert(*object);
		if (parent) parent->children[address] = this;
		

	}
	std::set<OctStructObject> objects;
	std::string				  address;
	OctStructTreeItem*		  parent;
	rl::BoundingBox			  boundingBox;
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
	std::unordered_map<void*, OctStructTreeItem*> octStructTreeItemMap;
	OctStruct		   octStruct;
	OctStructTreeItem* root;

	void addObject(void* obj, const BoundingBox& bbox);
	void moveObject(void* obj, const BoundingBox& bbox);
	std::vector<void*> getObjects(const BoundingBox& bbox);
	std::vector<void*> getObjects(const std::string& address);
	void removeObject(void* obj);

	std::string getBoundingBoxAddress(const BoundingBox& bbox);

	bool hitTest(const rl::Ray& ray, void* data);

	std::string toStr();
};





}