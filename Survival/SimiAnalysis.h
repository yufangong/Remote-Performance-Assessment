
#ifndef SIMIANALYSIS_H
#define SIMIANALYSIS_H
///////////////////////////////////////////////////////////////////////////////
// SimiAnalysis.h - using to analyze node in node repository, compare them   //
//                  to find out similar structure so that find out similar   //
//                  regions between or in files                              //
// ver 1.0                                                                   //
// Language:        Visual C++, Visual Studio 2013                           //
// platform:        Macbook pro, Parallels Desktop, Windows 7 profession     //
// Application:     spring 2014 Projects                                     //
// student:         Yufan Gong, Syracuse University                          //
//                  (315) 247-3777, ygong01@syr.edu                          //
///////////////////////////////////////////////////////////////////////////////
/*/
*
* Package Operations:
* ===================
* This package contains one class: Simianalysis. This class used to analyze node in node repository, 
* compare them to find out similar structure so that find out similarregions between or in files 
*
* Public Interface:
* =================
* void SortRepository();
* void showNodes();
* void ComplexityCompare(bool repeatless);
* std::map<Node*, std::vector<Node*>>& getResults();
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

#include "Collection.h"
#include "Element.h"
#include <map>
#include <algorithm>
class SimiAnalysis
{
public:
	SimiAnalysis();
	~SimiAnalysis();
	void SortRepository();
	void showNodes();
	void ComplexityCompare(bool repeatless);
	std::map<Node*, std::vector<Node*>>& getResults();
private:
	bool Condition(Node* node1, Node* node2);
	bool NodeAnalysis(Node* node1, Node* node2, bool repeatless);
	void detectRepeat(Node* node1, Node* node2);
	void storeRepeat(Node* node1, Node* node2);
	void eraseFromVector(Node* node);
	std::vector<Node*>& node_repository;
	std::vector<Node*>::iterator it;
	std::map<Node*, std::vector<Node*>> simiNodesIncluded;
	std::map<Node*, std::vector<Node*>> Results;
	struct descSort
	{
		bool operator()(Node* node1, Node* node2)
		{
			return node1->value().complexity > node2->value().complexity;
		}
	}descendentSort;
};

#endif