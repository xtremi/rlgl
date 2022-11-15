#pragma once
#include "rlOctreeStruct.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <set>

namespace rl{

	class OctreeObject {
	public:
		void* data = nullptr;
		BoundingBox bbox;

		bool operator<(const OctreeObject& rhs) const {
			return data < rhs.data;
		}
		bool operator==(const OctreeObject& rhs) const {
			return data == rhs.data;
		}

	};

class OctreeItem {
public:
	OctreeItem(
		OctreeItem*			   _parent,
		const std::string&	   _address, 
		const rl::BoundingBox& _boundingBox,
		const OctreeObject* object = nullptr)
		: address{ _address }, parent{_parent}, boundingBox{ _boundingBox }
	{
		if (object) objects.insert(*object);
		if (parent) parent->children[address] = this;
	}
	std::set<OctreeObject> objects;
	std::string			   address;
	OctreeItem*			   parent;
	rl::BoundingBox		   boundingBox;
	std::unordered_map<std::string, OctreeItem*> children;

	OctreeItem* insertObject(
		const OctreeObject& object, 
		const std::string&     addr,
		const rl::OctreeStruct&   octStruct);

	std::string toStr(std::string& str, int& level);
	std::string toStr();

};



class Octree {
public:
	Octree(){}
	Octree(const OctreeStruct& _octStruct) : octStruct{_octStruct}
	{
		if (root) delete root;

		root = new OctreeItem(nullptr, "0", 
			BoundingBox::createCubeBoundingBox(octStruct.center, octStruct.size * 2.f), nullptr);
	}
	OctreeStruct octStruct;
	std::unordered_map<void*, OctreeItem*> octStructTreeItemMap;
	OctreeItem* root = nullptr;

	OctreeItem*		   addObject(void* obj, const BoundingBox& bbox);
	void			   moveObject(void* obj, const BoundingBox& bbox);
	std::vector<void*> getObjects(const BoundingBox& bbox);
	std::vector<void*> getObjects(const std::string& address);
	void               removeObject(void* obj);
	bool               hitTest(const rl::Ray& ray, void** data);
	std::string        getBoundingBoxAddress(const BoundingBox& bbox);

	void			   callOnAllOctTreeObject(void (*func)(void*, const BoundingBox&, void*), void* customData = nullptr);
	void			   callOnOctTreeObjects(
		bool (*funcTestOctreeNode)(OctreeItem*, void*), 
		void (*funcDoOnNegativeTest)(void*),
		void* customData = nullptr);

	std::string toStr();

protected:
	bool hitTest(const OctreeItem* item, const rl::Ray& ray, void** data);
	void callOnOctTreeObjects2(
		OctreeItem* octTreeNode,
		bool (*funcTestOctreeNode)(OctreeItem*, void*),
		void (*funcDoOnNegativeTest)(void*),
		void* customData = nullptr);
	void callOnAllChildrenObjects(
		OctreeItem* octTreeNode,
		void (*func)(void*));

};





}