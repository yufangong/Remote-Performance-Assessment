#include "SimiAnalysis.h"

SortRepository::SortRepository() :node_repository(Collection().getNodeRepo())
{	
}

SortRepository::~SortRepository()
{
}

void SortRepository::SortRepo()
{
	randomQuickSort(node_repository, 0, node_repository.size() - 1);
}

void SortRepository::quickSort(std::vector<Node*>& node_repo, size_t p, size_t r)
{
	size_t i;
		if (p < r)
		{
			i = partition(node_repo, p, r);
			quickSort(node_repo, 0, i - 1);
			quickSort(node_repo, i + 1, r);
		}
}

void SortRepository::swap(std::vector<Node*>& node_repo, size_t a, size_t b)
{
	Node *temp = node_repo[a];
	node_repo[a] = node_repo[b];
	node_repo[b] = temp;
}

size_t SortRepository::partition(std::vector<Node*>& node_repo, size_t p, size_t r)
{
	size_t i = p - 1, j;
	for (j = p; j < r; j++)
	{
		if (node_repo[j]->value().complexity >= node_repo[r]->value().complexity)
		{
			i++;
			swap(node_repo, i, j);
		}
	}
	swap(node_repo, i + 1, r);
	return i + 1;
}

size_t SortRepository::Random(size_t min, size_t max)
{
	size_t randnum;
	randnum = rand() % (max - min) + 1;
	//randnum = (min + (size_t)(((float)rand() / RAND_MAX)*(max - min)));
	return randnum;
}

size_t SortRepository::randomPartition(std::vector<Node*>& node_repo, size_t p, size_t r)
{
	size_t i = Random(p, r);
	swap(node_repo, i, r);
	return partition(node_repo, p, r);
}

void SortRepository::randomQuickSort(std::vector<Node*>& node_repo, size_t p, size_t r)
{
	size_t i;
	if (p < r)
	{
		i = randomPartition(node_repo, p, r);
		quickSort(node_repo, 0, i - 1);
		quickSort(node_repo, i + 1, r);
	}
}

void SortRepository::showNodes()
{
	std::cout << "\n\n";
	for (auto node : node_repository)
	{
		std::cout << std::endl << node->value().name << "\t" << node->value().type << "\t" 
			<< node->value().file << "\t" << node->value().complexity;
	}
}

SimiAnalysis::SimiAnalysis() :node_repository(Collection().getNodeRepo())
{
}

SimiAnalysis::~SimiAnalysis()
{
}

void SimiAnalysis::ComplexityCompare()
{
	for (size_t i = 0; i < node_repository.size(); i++)
	{
		for (size_t j = i + 1; j < node_repository.size(); j++)
		{
			if (Condition(node_repository[i], node_repository[j]))
			{
				if (NodeAnalysis(node_repository[i], node_repository[j]))
				{
					Results[node_repository[i]] = node_repository[j];
				}
			}
			else break;
		}
	}
}
void SimiAnalysis::detectRepeat(Node* node1, Node* node2)
{
	std::map<Node*, std::vector<Node*>>::iterator mapIt1, mapIt2;
	mapIt1 = simiNodesIncluded.find(node1);
	if (mapIt1 != simiNodesIncluded.end())
	{
		for (size_t i = 0; i < (*mapIt1).second.size(); i++)
			Results[node2] = (*mapIt1).second[i];
		//simiNodesIncluded[node2] = node1;

	}
	mapIt2 = simiNodesIncluded.find(node2);
	if (mapIt2 != simiNodesIncluded.end())
	{
		for (size_t i = 0; i < (*mapIt1).second.size(); i++)
			Results[node1] = (*mapIt2).second[i];
		//simiNodesIncluded[node2] = node1;
	}
}
void SimiAnalysis::showResults()
{
	for (auto result : Results)
	{
		if (result.first->value().startline == 0 && result.second->value().startline == 0)
		{
			std::cout << "\n\n " << std::string(50, '=') << " \n The whole structure of these two files are similar : \n "
				<< std::string(50, '=') << "\n " << result.first->value().name << "\n WITH \n " << result.second->value().name;
		}
		else
		{
			std::cout << "\n\n\n ================\n" 
				<< " Similar Region: \n" 
				<< " ================\n";

			std::cout << " File : " << result.first->value().file << "\n Region:\tStartline: "
				<< result.first->value().startline << "\n\t\tEndline: " << result.first->value().endline
				<< "\n Scope Type: " << result.first->value().type << "\t Scope Name: " << result.first->value().name
				<< "\n\n WITH \n\n"
				<< " File : " << result.second->value().file << "\n Region:\tStartline: "
				<< result.second->value().startline << "\n\t\tEndline: " << result.second->value().endline
				<< "\n Scope Type: " << result.second->value().type << "\t Scope Name: " << result.second->value().name;
		}
	}
}
bool SimiAnalysis::Condition(Node* node1, Node* node2)
{
	if (node1->value().complexity >=3 && node2->value().complexity >=3 && node1->value().complexity == node2->value().complexity)
		// && node1->value().file != node2->value().file)
		return true;
	else
		return false;
	
}
bool SimiAnalysis::NodeAnalysis(Node* node1, Node* node2)
{
	if (node1->value().type != node2->value().type)
		return false; // found something so stop walk
	if (node1->value().file == node2->value().file)
		return false;
	size_t size1 = node1->value().endline - node1->value().startline;
	size_t size2 = node2->value().endline - node2->value().startline;
	if (size1 - size2 >= 20 || size2 - size1 >= 20)
		return false;
	node1->clearMarks();
	node2->clearMarks();
	Node* pTemp1;
	Node* pTemp2;
	while ((pTemp1 = node1->nextUnmarkedChild()) && (pTemp2 = node2->nextUnmarkedChild()))
	{
		if (!NodeAnalysis(pTemp1, pTemp2))
		{
			return false;
		}
		//if (pTemp1->value().complexity >=3 )
		//	simiNodesIncluded[pTemp1].push_back(pTemp2);
		//it = find(node_repository.begin(), node_repository.end(), pTemp2);
		//if (it != node_repository.end())
		//	node_repository.erase(it);
	}
	return true;
}

#ifdef TEST_SIMIANALYSIS
//#include <stdlib.h>
//#include <stdio.h>
//#include <time.h>
int main()
{
	using Node = TMTree::MNode<element>;
	std::vector<Node*> test;
	element e1, e2, e3, e4, e5;
	e1.complexity = 2;
	e2.complexity = 5;
	e3.complexity = 4;
	e4.complexity = 1;
	e5.complexity = 2;
	Node *n1 = new Node(e1);
	Node *n2 = new Node(e2);
	Node *n3 = new Node(e3);
	Node *n4 = new Node(e4);
	Node *n5 = new Node(e5);
	test.push_back(n1);
	test.push_back(n2);
	test.push_back(n3);
	test.push_back(n4);
	test.push_back(n5);
	for (size_t m = 0; m < test.size(); m++)
	{
		std::cout << test[m]->value().complexity << std::endl;
	}
	SortRepository sort;
	//srand((unsigned)time(NULL));
	sort.randomQuickSort(test, 0, test.size()-1);
	for (size_t m = 0; m < test.size(); m++)
	{
		std::cout << test[m]->value().complexity << std::endl;
	}
}
#endif