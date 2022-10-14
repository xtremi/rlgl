#include "rlOctStructTree.h"
#include "rlRayIntersection.h"

using namespace rl;

/*
------------------------------------------------------------------------------
	1. This item has a child with the exact same address
------------------------------------------------------------------------------
Ex1:	[root]       insert [1234]  ---> 	  [root]     
	[1234]* [3455]*  				   	 [1234]**   [3455]*
------------------------------------------------------------------------------

------------------------------------------------------------------------------
	2. An OctreeItem with matching start of address exist (recursive):
------------------------------------------------------------------------------
Ex2.1	[root]       insert [12345]  --->  match [1234](5) with [1234] 
	[1234]* [3455]*  				   	   call insertObject on [1234]  --> go to (5.)

Ex2.2
		  [root]       insert [12345]  --->  match [12](345) with [12]
	  [12]     [3455]*  				   	   call insertObject on [12]
  [123]*[124]*                               match [123](45) with [123]
[1234]*                                        call insertObject on [123]
                                             match [1234](5) with [1234]
											   call insertObject on [1234]  --> go to (5.)

------------------------------------------------------------------------------
	3. If the common address is already the address of this item, just add it as child here:
	Check if changes need to made to tree
------------------------------------------------------------------------------

Ex3	     [root]       insert [1124] on [11]  ---> Common address [11] of [1123] and [1124]
	  [11]   [2341]* 				   	          [11] is the address of the current item
 [1123]* [1133]*					              Set [1124] as child of [11]



------------------------------------------------------------------------------
	4a. Make new parent and move existing child under it:
------------------------------------------------------------------------------
Ex4a 	[root]       insert [1133]  --->  Common address [11] of [1123] and [1133]
	[1123]* [2341]*  				   	  Create new item/node [11] as child of [1123]
	                                      Set [1123] and [1133] as children of [11]
	     [root] 
	  [11]   [2341]*
 [1123]* [1133]*


 ------------------------------------------------------------------------------
	4b. Make new parent and move existing child under it:
------------------------------------------------------------------------------
Ex4b 	[root]       insert [11]  --->    Common address [11] of [1123] and [11]
	[1123]* [2341]*  				   	  Create new item/node [11] (with inserted object) as child of [1123]
										  Set [1123] as children of [11]
		 [root]
	  [11]*  [2341]*
 [1123]* 



 ------------------------------------------------------------------------------
	5. No child to check - just add as child of current item
------------------------------------------------------------------------------
Ex5
    		  [root]       insert [12345] on [1234] ---> create child of [1234]
    	  [12]     [3455]*  				   	          
      [123]*[124]*                               
    [1234]*          

    		  [root]
    	  [12]     [3455]*
      [123]*[124]*
    [1234]*
[12345]*


*/
OctreeItem* OctreeItem::insertObject(
	const OctreeObject& object,
	const std::string&     objAddress,
	const rl::OctreeStruct&   octStruct)
{

	/*
		1. This item has a child with the exact same address
	*/
	auto it = children.find(objAddress);
	if (it != children.end()) {
		it->second->objects.insert(object);
		return it->second;
	}

	/*
		2. An OctreeItem with matching start of address exist (recursive):
	*/
	std::string subAddress;	
	for (char c : objAddress) {
		subAddress += c;
		it = children.find(subAddress);
		if (it != children.end()) {
			return it->second->insertObject(object, objAddress, octStruct);
		}
	}

	BoundingBox itemBbox = octStruct.addressBoundingBox(objAddress);

	/*
		Check common part of address with children
	*/
	for (auto it = children.begin(); it != children.end(); it++) {
		std::string commonAddress = getCommonAddress(objAddress, it->second->address);
		if (!commonAddress.empty()) {

			//3.  If the common address is already the address of this item, just add it as child here:
			if (commonAddress == address) {
				return new OctreeItem(this, objAddress, itemBbox, &object);
			}


			//4a/4b Make new parent and move existing child under it:
			BoundingBox commonItemBbox = octStruct.addressBoundingBox(commonAddress);
			OctreeItem* newCommonParent = new OctreeItem(this, commonAddress, commonItemBbox);

			OctreeItem* existingItem = it->second;
			children.erase(existingItem->address);		  //remove from child
			newCommonParent->children[existingItem->address] = existingItem;
			existingItem->parent = newCommonParent;

			//Create new item (unless the address of the new object is the same as the "common address")
			/*4a*/
			if(commonAddress != objAddress){
				return new OctreeItem(newCommonParent, objAddress, itemBbox, &object);
			}
			/*4b*/
			else {
				newCommonParent->objects.insert(object);
				return newCommonParent;
			}
		}
	}

	/*
		5. Just add as child of current item
	*/
	return new OctreeItem(this, objAddress, itemBbox, &object);
}

std::string OctreeItem::toStr(std::string& str, int& level) {
	
	for (int i = 0; i < level; i++) {
		str += "  ";
	}
	str += toStr() + "\n";
	level++;
	
	for (auto it = children.begin(); it != children.end(); it++) {
		int branchLevel = level;
		it->second->toStr(str, branchLevel);
	}


	return str;
}

std::string OctreeItem::toStr() {
	std::string str = "";
	str += "[" + address + "] - ";
	for(const OctreeObject& obj : objects){
		str += std::to_string((uint64_t)obj.data) + ", ";
	}
	if(!objects.empty()){
		str.pop_back(); str.pop_back();
	}
	return str;
}

std::string Octree::getBoundingBoxAddress(const BoundingBox& bbox) {
	std::string addrMin = octStruct.getOctAddress(bbox.minC);
	std::string addrMax = octStruct.getOctAddress(bbox.maxC);
	return getCommonAddress(addrMin, addrMax);
}

OctreeItem* Octree::addObject(void* obj, const BoundingBox& bbox) {
	std::string address = getBoundingBoxAddress(bbox);
	BoundingBox treeItemBbox = octStruct.addressBoundingBox(address);
	OctreeItem* item = root->insertObject({obj, bbox}, address, this->octStruct);
	this->octStructTreeItemMap[obj] = item;
	return item;
}

void Octree::removeObject(void* obj) {

	auto it = octStructTreeItemMap.find(obj);
	if (it != octStructTreeItemMap.end()) {
		OctreeItem* item = it->second;
		for (const OctreeObject& osObj : item->objects) {
			if (osObj.data == obj) {
				item->objects.erase(osObj);
				break;
			}
		}	

		octStructTreeItemMap.erase(obj);

		//Recursively delete parents if parent no children or objects (but not sure if a parent of a parent will ever be empty?)
		while (item->objects.size() == 0 && item->children.size() == 0){
			OctreeItem* parent = item->parent;
			if (!parent /*its the root*/) return;
			parent->children.erase(item->address);
			delete item;
			item = parent;
		}

		/*
			Clean up so no uncessary item in tree branch
			Example:
				    root
			[11] (1 obj)  [22]
         [111] (1 obj)

		 Remove obj in [11]
					root                      root
			[11]         [22] ---->   [111] (1 obj)  [22]
		 [111] (1 obj)


			*/
		if (item->objects.size() == 0 && item->children.size() == 1) {
			OctreeItem* parent    = item->parent;
			if (!parent /*its the root*/) return;

			parent->children.erase(item->address);

			OctreeItem* onlyChild = item->children.begin()->second;
			onlyChild->parent = parent;

			delete item;

			parent->children[onlyChild->address] = onlyChild;
		}

	}
}

/*TODO: more effective*/
void Octree::moveObject(void* obj, const BoundingBox& bbox) {
	removeObject(obj);
	addObject(obj, bbox);
}

/*TODO*/
std::vector<void*> Octree::getObjects(const BoundingBox& bbox) {
	std::vector<void*> objects;
	return objects;
}

/*TODO*/
std::vector<void*> Octree::getObjects(const std::string& address) {
	std::vector<void*> objects;
	return objects;
}

std::string Octree::toStr() {
	std::string str;
	int level = 0;
	return root->toStr(str, level);
}

bool Octree::hitTest(const OctreeItem* item, const rl::Ray& ray, void** data) {

	auto it = item->children.begin();
	for (it; it != item->children.end(); it++) {
		
		if (rl::rayIntersection(ray, it->second->boundingBox)) {

			//Objects owned by item				
			auto itObj = it->second->objects.begin();
			for (itObj; itObj != it->second->objects.end(); itObj++) {

				if (rl::rayIntersection(ray, itObj->bbox)) {
					*data = itObj->data;
					return true;
				}

			}

			if (hitTest(it->second, ray, data)) {
				return true;
			}
		}
	}
	return false;

}


bool Octree::hitTest(const rl::Ray& ray, void** data) {
	return hitTest(root, ray, data);
}