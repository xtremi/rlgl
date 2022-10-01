#include "rlOctStructTree.h"

using namespace rl;

/*
------------------------------------------------------------------------------
	1. This item has a child with the exact same address
------------------------------------------------------------------------------
Ex1:	[root]       insert [1234]  ---> 	  [root]     
	[1234]* [3455]*  				   	 [1234]**   [3455]*
------------------------------------------------------------------------------

------------------------------------------------------------------------------
	2. An OctStructTreeItem with matching start of address exist (recursive):
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
OctStructTreeItem* OctStructTreeItem::insertObject(void* object, const std::string& objAddress) {

	/*
		1. This item has a child with the exact same address
	*/
	auto it = children.find(objAddress);
	if (it != children.end()) {
		it->second->objects.insert(object);
		return it->second;
	}

	/*
		2. An OctStructTreeItem with matching start of address exist (recursive):
	*/
	std::string subAddress;	
	for (char c : objAddress) {
		subAddress += c;
		it = children.find(subAddress);
		if (it != children.end()) {
			return it->second->insertObject(object, objAddress);
		}
	}

	/*
		Check common part of address with children
	*/
	for (auto it = children.begin(); it != children.end(); it++) {
		std::string commonAddress = getCommonAddress(objAddress, it->second->address);
		if (!commonAddress.empty()) {

			//3.  If the common address is already the address of this item, just add it as child here:
			if (commonAddress == address) {
				return new OctStructTreeItem(this, objAddress, object);
			}

			//4a/4b Make new parent and move existing child under it:
			OctStructTreeItem* newCommonParent = new OctStructTreeItem(this, commonAddress);

			OctStructTreeItem* existingItem = it->second;
			children.erase(existingItem->address);		  //remove from child
			newCommonParent->children[existingItem->address] = existingItem;
			existingItem->parent = newCommonParent;

			//Create new item (unless the address of the new object is the same as the "common address")
			/*4a*/
			if(commonAddress != objAddress){
				return new OctStructTreeItem(newCommonParent, objAddress, object);
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
	return new OctStructTreeItem(this, objAddress, object);
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

		octStructTreeItemMap.erase(obj);

		//Recursively delete parents if parent no children or objects (but not sure if a parent of a parent will ever be empty?)
		while (item->objects.size() == 0 && item->children.size() == 0){
			OctStructTreeItem* parent = item->parent;
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
			OctStructTreeItem* parent    = item->parent;
			if (!parent /*its the root*/) return;

			parent->children.erase(item->address);

			OctStructTreeItem* onlyChild = item->children.begin()->second;
			onlyChild->parent = parent;

			delete item;

			parent->children[onlyChild->address] = onlyChild;
		}

	}
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