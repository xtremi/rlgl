#include "rlOctStructTree.h"

using namespace rl;

/*
	Will always insert - return nullptr if not created new
	OctStructTreeItem otherwise returns ptr to created
	OctStructTreeItem.
*/
OctStructTreeItem* OctStructTreeItem::insertObject(void* object, const std::string& addr) {

	std::string subAddress;
	//OctStructTreeItem* currentItem = this;
	for (char c : addr) {
		subAddress += c;
		if (children.find(subAddress) != children.end()) {
			return children[subAddress]->insertObject(object, addr); //not correct if complete matching address
		}
	}

	//Check if changes need to made to tree
	for (auto it = children.begin(); it != children.end(); it++) {
		std::string commonAddress = getCommonAddress(address, it->second->address);
		if (!commonAddress.empty()) {
			OctStructTreeItem* existingItem = it->second;
			children.erase(existingItem->address);		  //remove from child

			OctStructTreeItem* newCommonParent = new OctStructTreeItem(commonAddress);
			children[commonAddress] = newCommonParent;

			OctStructTreeItem* newItem = new OctStructTreeItem(addr, object);
			newCommonParent->children[address] = newItem;
			newCommonParent->children[existingItem->address] = existingItem;
			return newItem;
		}
	}

	OctStructTreeItem* newItem = new OctStructTreeItem(address, object);
	children[addr] = newItem;
	return newItem;
}



std::string OctStructTree::getBoundingBoxAddress(const OctCoord& bboxMin, const OctCoord& bboxMax) {
	std::string addrMin = octStruct.getOctAddress(bboxMin);
	std::string addrMax = octStruct.getOctAddress(bboxMax);
	return getCommonAddress(addrMin, addrMax);
}

void OctStructTree::removeObject(void* obj) {
	std::string address = octStructAddressMap[obj];
	octStructAddressMap.erase(obj);

}


void OctStructTree::addObject(void* obj, const OctCoord& bboxMin, const OctCoord& bboxMax) {
	std::string address = getBoundingBoxAddress(bboxMin, bboxMax);

	root->insertObject(obj, address);

}