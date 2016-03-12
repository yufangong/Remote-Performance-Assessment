/////////////////////////////////////////////////////////////////////////////
// Collection.cpp-creates nodes and build struture of nodes, push          //
//                them intoshared node repository                          //
// ver 1.0                                                                 //
// Language:      Visual C++, Visual Studio 2013                           //
// platform:      Macbook pro, Parallels Desktop, Windows 7 profession     //
// Application:   spring 2014 Projects                                     //
// student:       Yufan Gong, Syracuse University                          //
//                (315) 247-3777, ygong01@syr.edu                          //
/////////////////////////////////////////////////////////////////////////////

#include "Collection.h"

//----< declare static share node repository >-----------

std::vector<Node*> Collection::NodeRepository;

//----< push node into Collection >-----------

void Collection::pushNode(const element& elem)
{
	Node* pNode = new Node(elem);
	if (elem.type == "root")
	{
		currNode = pNode;
		pRoot = pNode;
		NodeRepository.push_back(currNode);
	}
	else
	{
		currNode->addChild(pNode);
		currNode = pNode;
	}
}

//----< pop node and back to parent node >-----------

element Collection::popNode(size_t endline)
{
	currNode->value().endline = endline;
	element elem;
	lastPopNode = currNode;
	currNode->value().complexity = currNode->GetOffNum();
	if (!currNode->getPoped())
	{
		NodeRepository.push_back(currNode);
	}
	Node* temp = currNode->findParent();
	if (temp)
	{	
		elem = currNode->value();
		currNode = temp;
		if (currNode->getBraceless())
			popNode(endline);
	}
	if (temp->value().type == "root")
	{
		if (temp->value().endline < endline)
		{
			temp->value().endline = endline;
		}
	}
	return elem;
}
//----< handle braceless situation >-----------

void Collection::markBracelessNode(size_t count)
{
	lastPushbraceless = currNode;
	tempNode = currNode;
	for (size_t i = 1; i < count; i++)
	{
		tempNode = tempNode->markParent();
	}
}
//----< handle else with braceless scope ahead >-----------

void Collection::PushElse(const element& elem)
{
	Node* pNode = new Node(elem);
	if (lastPopNode->getBraceless())
	{
		lastPushbraceless->getParent()->addChild(pNode);
		currNode = pNode;
	}
	else
	{
		currNode->addChild(pNode);
		currNode = pNode;
	}
}
//----< push root node to node repository>-----------

void Collection::setFiles()
{
	pRoot->value().complexity = pRoot->GetOffNum();
	//createXml(pRoot);
}
//----< put node repository to simianalysis >-----------

std::vector<Node*>& Collection::getNodeRepo()
{
	return NodeRepository;
}

//----< clear node repository >-----------
void Collection::clearNodeRepo()
{
	NodeRepository.clear();
}

//----< create xml about each file structure to test if the construction is correct >-----------

bool Collection::createXml(Node* pStart)
{
	xml.indent();
	xml.addDeclaration();
	xmlWalk(pStart);
	xml.end();
	std::cout << xml.xml().c_str();
	std::cout << std::endl;
	return false;
}
//----< help create xml >-----------

bool Collection::xmlWalk(Node* pStart)
{
	std::stringstream ss;
	xml.start(pStart->value().name);
	xml.addAttribute("type", pStart->value().type);
	ss << pStart->GetOffNum();
		std::string cc = ss.str();
		xml.addAttribute("complexity", cc);
		ss.str("");
	pStart->clearMarks();
	Node* pTemp;
	while (pTemp = pStart->nextUnmarkedChild())
	{
		if (!xmlWalk(pTemp))
		{
			return false;
		}
		xml.end();
	}
	return true;
}
#ifdef TEST_COLLECTION
int main()
{
	using Node = TMTree::MNode<element>;
	element e1, e2, e3, e4, e5, E1, E2, E3, E4, E5;
	e1.name = "1";
	e1.type = "root";
	e1.endline = 0;
	e2.name = "2";
	e3.name = "3";
	e4.name = "4";
	e5.name = "5";
	Collection collect;
	collect.pushNode(e1);
	collect.pushNode(e2);
	E1 = collect.popNode(1);
	collect.PushElse(e3);
	collect.pushNode(e4);
	E2 = collect.popNode(2);
	collect.pushNode(e5);
	E3 = collect.popNode(3);
	E4 = collect.popNode(4);
	collect.setFiles();
	std::vector<Node*>& test = collect.getNodeRepo();
	for (size_t m = 0; m < test.size(); m++)
	{
		std::cout << test[m]->value().name << std::endl;
		std::cout << test[m]->value().endline << std::endl;
	}
}
#endif