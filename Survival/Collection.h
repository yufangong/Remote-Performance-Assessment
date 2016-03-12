#ifndef COLLECTION_H
#define COLLECTION_H
/////////////////////////////////////////////////////////////////////////////
// Collection.h - creates nodes and build struture of nodes, push          //
//                them intos hared node repository                         //
// ver 1.0                                                                 //
// Language:      Visual C++, Visual Studio 2013                           //
// platform:      Macbook pro, Parallels Desktop, Windows 7 profession     //
// Application:   spring 2014 Projects                                     //
// student:       Yufan Gong, Syracuse University                          //
//                (315) 247-3777, ygong01@syr.edu                          //
/////////////////////////////////////////////////////////////////////////////
/*
*
* Package Operations:
* ===================
* This package contains one class: collection. This class used to push nodes into 
* shared node repository build trees
*
* Public Interface:
* =================
* void pushNode(const element& elem);
* element popNode(size_t endline);
* void PushElse(const element& elem);
* void setFiles();
* std::vector<Node*>& getNodeRepo();
* void markBracelessNode(size_t count);
*
* Build Command:
* ==============
* devenv ScopeStack.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 : 21 Mar 14
* - first release
*/


#include <vector>



#include <string>
#include "Element.h"
#include "Display.h"
#include "XmlWriter.h"

using Node = TMTree::MNode<element>;

class Collection
{
public:
	void pushNode(const element& elem);
	element popNode(size_t endline);
	void PushElse(const element& elem);
	void setFiles();
	std::vector<Node*>& getNodeRepo();
	void markBracelessNode(size_t count);
	void clearNodeRepo();
private:
	bool createXml(Node* pStart);
	bool xmlWalk(Node* pStart);
	std::string filename;
	Node* pRoot;
	static std::vector<Node*> NodeRepository; //share this vector to store all nodes of each file
	Node* currNode;
	Node* tempNode;
	Node* lastPushbraceless;
	Node* lastPopNode;
	XmlWriter xml;
};
#endif
