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
		const rl::BoundingBox& _boundingBox,
		const OctStructObject* object = nullptr)
		: address{ _address }, parent{_parent}, boundingBox{ _boundingBox }
	{
		if (object) objects.insert(*object);
		if (parent) parent->children[address] = this;
		

	}
	std::set<OctStructObject> objects;
	std::string				  address;
	OctStructTreeItem*		  parent;
	rl::BoundingBox			  boundingBox;
	std::unordered_map<std::string, OctStructTreeItem*> children;

	OctStructTreeItem* insertObject(
		const OctStructObject& object, 
		const std::string&     addr,
		const rl::BoundingBox& _boundingBox);

	std::string toStr(std::string& str, int& level);
	std::string toStr();

};



class OctStructTree {
public:
	OctStructTree(){}
	OctStructTree(const OctStruct& _octStruct) : octStruct{_octStruct}
	{
		if (root) delete root;

		root = new OctStructTreeItem(nullptr, "0", 
			BoundingBox::createCubeBoundingBox(octStruct.center, octStruct.size * 2.f), nullptr);
	}
	std::unordered_map<void*, OctStructTreeItem*> octStructTreeItemMap;
	OctStruct		   octStruct;
	OctStructTreeItem* root = nullptr;

	OctStructTreeItem* addObject(void* obj, const BoundingBox& bbox);
	void			   moveObject(void* obj, const BoundingBox& bbox);
	std::vector<void*> getObjects(const BoundingBox& bbox);
	std::vector<void*> getObjects(const std::string& address);
	void               removeObject(void* obj);
	bool               hitTest(const rl::Ray& ray, void** data);
	std::string        getBoundingBoxAddress(const BoundingBox& bbox);

	std::string toStr();

private:
	bool hitTest(const OctStructTreeItem* item, const rl::Ray& ray, void** data);

};





}