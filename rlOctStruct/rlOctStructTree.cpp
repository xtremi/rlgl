#include "rlOctStructTree.h"

using namespace rl;

/*
	
*/
OctStructTreeItem* OctStructTreeItem::insertObject(void* object, const std::string& objAddress) {

	//This item has a child with the exact same address 
	auto it = children.find(objAddress);
	if (it != children.end()) {
		it->second->objects.insert(object);
		return it->second;
	}

	//An OctStructTreeItem with matching start of address exist (recursive):
	std::string subAddress;	
	for (char c : objAddress) {
		subAddress += c;
		if (children.find(subAddress) != children.end()) {
			return children[subAddress]->insertObject(object, objAddress); //will re-search from beginning of address...
		}
	}

	/*
	Check if changes need to made to tree
	Example 1: 
		     root
		[1123]  [2341]
	
	- Insert [1133]

	         root
		 [11]   [2341] 
    [1123] [1133]  

	Example 2:
			 root
		[1123]  [2341]

	- Insert [11]

			 root
		 [11]   [2341]
	[1123]
	*/
	for (auto it = children.begin(); it != children.end(); it++) {
		std::string commonAddress = getCommonAddress(objAddress, it->second->address);
		if (!commonAddress.empty()) {

			//If the common address is already the address of this item, just add it as child here:
			if (commonAddress == address) {
				OctStructTreeItem* newItem = new OctStructTreeItem(objAddress, object);
				this->children[objAddress] = newItem;
				return newItem;
			}

			//Make new parent and move existing child under it:
			OctStructTreeItem* newCommonParent = new OctStructTreeItem(commonAddress);
			children[commonAddress] = newCommonParent;

			OctStructTreeItem* existingItem = it->second;
			children.erase(existingItem->address);		  //remove from child
			newCommonParent->children[existingItem->address] = existingItem;

			//Create new item (unless the address of the new object is the same as the "common address")
			if(commonAddress != objAddress){
				OctStructTreeItem* newItem = new OctStructTreeItem(objAddress, object);
				newCommonParent->children[objAddress] = newItem;
				return newItem;
			}
			else {
				newCommonParent->objects.insert(object);
				return newCommonParent;
			}
		}
	}

	OctStructTreeItem* newItem = new OctStructTreeItem(objAddress, object);
	children[objAddress] = newItem;
	return newItem;
}

std::string OctStructTreeItem::toStr(std::string& str, int& level) {
	
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

std::string OctStructTreeItem::toStr() {
	std::string str = "";
	str += "[" + address + "] - ";
	for(void* obj : objects){
		str += std::to_string((uint64_t)obj) + ", ";
	}
	if(!objects.empty()){
		str.pop_back(); str.pop_back();
	}
	return str;
}

std::string OctStructTree::getBoundingBoxAddress(const OctCoord& bboxMin, const OctCoord& bboxMax) {
	std::string addrMin = octStruct.getOctAddress(bboxMin);
	std::string addrMax = octStruct.getOctAddress(bboxMax);
	return getCommonAddress(addrMin, addrMax);
}


void OctStructTree::addObject(void* obj, const OctCoord& bboxMin, const OctCoord& bboxMax) {
	std::string address = getBoundingBoxAddress(bboxMin, bboxMax);
	OctStructTreeItem* item = root->insertObject(obj, address);
	this->octStructTreeItemMap[obj] = item;
}


/*TODO*/
void OctStructTree::removeObject(void* obj) {
	auto it = octStructTreeItemMap.find(obj);
	if (it != octStructTreeItemMap.end()) {
		OctStructTreeItem* item = it->second;
		item->objects.erase(obj);
		if (item->children.size() == 0 && item->objects.size() == 0) {

		}
	}
	std::string address = octStructAddressMap[obj];
	octStructAddressMap.erase(obj);
	//more clean up
}

/*TODO*/
void OctStructTree::moveObject(void* obj, const OctCoord& bboxMin, const OctCoord& bboxMax) {

}

/*TODO*/
std::vector<void*> OctStructTree::getObjects(const OctCoord& bboxMin, const OctCoord& bboxMax) {
	std::vector<void*> objects;
	return objects;
}

/*TODO*/
std::vector<void*> OctStructTree::getObjects(const std::string& address) {
	std::vector<void*> objects;
	return objects;
}

std::string OctStructTree::toStr() {
	std::string str;
	int level = 0;
	return root->toStr(str, level);
}