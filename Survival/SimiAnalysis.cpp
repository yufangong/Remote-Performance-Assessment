/////////////////////////////////////////////////////////////////////////////////
// SimiAnalysis.cpp - using to analyze node in node repository, compare them   //
//                    to find out similar structure so that find out similar   //
//                    regions between or in files                              //
// ver 1.0                                                                     //
// Language:          Visual C++, Visual Studio 2013                           //
// platform:          Macbook pro, Parallels Desktop, Windows 7 profession     //
// Application:       spring 2014 Projects                                     //
// student:           Yufan Gong, Syracuse University                          //
//                    (315) 247-3777, ygong01@syr.edu                          //
/////////////////////////////////////////////////////////////////////////////////
#include "SimiAnalysis.h"

//----< constructor to initialize node repository reference >-----------

SimiAnalysis::SimiAnalysis() :node_repository(Collection().getNodeRepo())
{
}

//----< destructor >-----------

SimiAnalysis::~SimiAnalysis()
{
}

//----< sort node repository by complexity >-----------

void SimiAnalysis::SortRepository()
{
	sort(node_repository.begin(), node_repository.end(), descendentSort);
}

//----< show nodes in repository >-----------

void SimiAnalysis::showNodes()
{
	std::cout << "\n\n";
	for (auto node : node_repository)
	{
		std::cout << std::endl << node->value().name << "\t" << node->value().type << "\t"
			<< node->value().file << "\t" << node->value().complexity;
	}
}

//----< compare two nodes structures >-----------

void SimiAnalysis::ComplexityCompare(bool repeatless)
{
	for (size_t i = 0; i < node_repository.size(); i++)
	{
		for (size_t j = i + 1; j < node_repository.size(); j++)
		{
			if (Condition(node_repository[i], node_repository[j]))
			{
				if (NodeAnalysis(node_repository[i], node_repository[j], repeatless))
				{
					Results[node_repository[i]].push_back(node_repository[j]);
					if (repeatless)
						detectRepeat(node_repository[i], node_repository[j]);
				}
			}
			else break;
		}
	}
	Collection().clearNodeRepo();
}

//----< if two nodes similar, check the map to find other nodes similar to them have been erased >-----------

void SimiAnalysis::detectRepeat(Node* node1, Node* node2)
{
	std::map<Node*, std::vector<Node*>>::iterator mapIt1, mapIt2;
	mapIt1 = simiNodesIncluded.find(node1);
	bool r1 = false;
	bool r2 = false;
	if (mapIt1 != simiNodesIncluded.end())
	{
		for (size_t i = 0; i < (*mapIt1).second.size(); i++)
			Results[node2].push_back((*mapIt1).second[i]);
		r1 = true;
	}
	mapIt2 = simiNodesIncluded.find(node2);
	if (mapIt2 != simiNodesIncluded.end())
	{
		for (size_t i = 0; i < (*mapIt2).second.size(); i++)
			Results[node1].push_back((*mapIt2).second[i]);
		r2 = true;
	}
	if (r1 && r2)
	{
		for (size_t i = 0; i < (*mapIt1).second.size(); i++)
		{
			for (size_t j = 0; j < (*mapIt2).second.size(); j++)
				Results[(*mapIt1).second[i]].push_back((*mapIt2).second[j]);
		}
	}
}

//----< get results >-----------

std::map<Node*, std::vector<Node*>>& SimiAnalysis::getResults()
{
	return Results;
}

//----< set compare conditions and break loop in compare function >-----------

bool SimiAnalysis::Condition(Node* node1, Node* node2)
{
	if (node1->value().complexity > 3 && node2->value().complexity > 3 && node1->value().complexity == node2->value().complexity)
		return true;
	else
		return false;
	
}
//----< compare nodes using DFS >-----------

bool SimiAnalysis::NodeAnalysis(Node* node1, Node* node2, bool repeatless)
{
	if (node1->value().type != node2->value().type)
		return false; 
	size_t size1 = node1->value().endline - node1->value().startline + 1;
	size_t size2 = node2->value().endline - node2->value().startline + 1;
	int diffsize = size1 - size2;
	if (diffsize > 20)
		return false;
	if (diffsize < -20)
		return false;
	node1->clearMarks();
	node2->clearMarks();
	Node* pTemp1;
	Node* pTemp2;
	while ((pTemp1 = node1->nextUnmarkedChild()) && (pTemp2 = node2->nextUnmarkedChild()))
	{
		if (!NodeAnalysis(pTemp1, pTemp2, repeatless))
		{
			return false;
		}
		if (repeatless)
			storeRepeat(pTemp1, pTemp2);
	}
	return true;
}

//----< two nodes similar, erase one and store another >-----------

void SimiAnalysis::storeRepeat(Node* pTemp1, Node* pTemp2)
{
	std::map<Node*, std::vector<Node*>>::iterator mapIt1, mapIt2;	
	bool r1 = false;
	bool r2 = false;
	mapIt1 = simiNodesIncluded.find(pTemp1);
	if (mapIt1 != simiNodesIncluded.end())
		r1 = true;
	mapIt2 = simiNodesIncluded.find(pTemp2);
	if (mapIt2 != simiNodesIncluded.end())
		r2 = true;
	if (!r2)
	{
		if (pTemp1->value().complexity > 3)
			simiNodesIncluded[pTemp1].push_back(pTemp2);
		eraseFromVector(pTemp2);
	}
	if (r2 && !r1)
	{
		simiNodesIncluded[pTemp2].push_back(pTemp1);
		eraseFromVector(pTemp1);
	}
	if (r1 && r2)
	{
		simiNodesIncluded[pTemp1].push_back(pTemp2);
		for (size_t i = 0; i < (*mapIt2).second.size(); i++)
		{
			simiNodesIncluded[pTemp1].push_back((*mapIt2).second[i]);
		}
		eraseFromVector(pTemp2);
	}
}

//----< erase one of similar node from node repository >-----------

void SimiAnalysis::eraseFromVector(Node* node)
{
	it = find(node_repository.begin(), node_repository.end(), node);
	if (it != node_repository.end())
		node_repository.erase(it);
}
#ifdef TEST_SIMIANALYSIS
//#include <stdlib.h>
//#include <stdio.h>
//#include <time.h>
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
	e1.complexity = 2;
	e2.complexity = 5;
	e3.complexity = 4;
	e4.complexity = 1;
	e5.complexity = 2;
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

	SimiAnalysis simi;
	simi.SortRepository();
	simi.showNodes();
	simi.ComplexityCompare(false);
	std::map<Node*, std::vector<Node*>>& results = simi.getResults();
	std::vector<std::string> files = { "file1", "file2" };
	Display dp;
	dp.showResults(results, files, false);
}
#endif