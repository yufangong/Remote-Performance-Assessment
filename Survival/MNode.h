
#ifndef MNODE_H
#define MNode_H
/////////////////////////////////////////////////////////////////////////////
// MNode.h   -  M-ary node class to use in M-ary tree                      //
// ver 2.0                                                                 //
// Language:      Visual C++, 2013                                         //
// platform:      Macbook pro, Parallels Desktop, Windows 7 profession     //
// Application:   spring 2014 Projects                                     //
// Author:        Jim Fawcett, CST 4-187, Syracuse University              //
//                (315) 443-3948, jfawcett@twcny.rr.com                    //
// Modified:      Yufan Gong, Syracuse University                          //
//                (315) 247-3777, ygong01@syr.edu                          //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* Provides a template-based M-ary Node class that holds a finite number
* of child nodes.
*
* Required Files:
* ===============
* MNode.h, MNode.cpp
*
* Build Process:
* ==============
* cl /D:TEST_MNODE MNode.cpp
*
* Maintenance History:
* ====================
* ver 2.0 : 21 Mar 14
* - add markbraceless function to mark current node's parent as braceless node
* - add setbraceless function and getbraceless function
* - add getparent function to just get current node's parent
* - add getPoped function to return bool poped
* - delete root functions and data member;
* ver 1.1 : 11 Feb 14
* - add function to set this node is a root of a tree and get function;
* - add function to set and get numbers of offsprings of a node;
* - add funciton to find parent node;
* ver 1.0 : 27 Feb 10
* - first release
*/


#include <iostream>
#include <vector>

namespace TMTree
{
	/////////////////////////////////////////////////////////////////////////
	// MNode<T> class

	template <typename T>
	class MNode
	{
	public:
		MNode(const T& val);
		MNode(const MNode<T>& node);
		~MNode();
		MNode<T>* clone();
		MNode<T>& operator=(const MNode<T>& node);
		T& value();
		void addChild(MNode<T>* pNode);
		bool removeChild(MNode<T>* pChild);
		size_t numChildren();
		MNode<T>* nextUnmarkedChild();
		void clearMarks();
		std::string ToString();
		MNode<T>* findParent();
		size_t GetOffNum();
		void setBraceless();
		bool getBraceless();
		MNode<T>* markParent();
		MNode<T>* getParent();
		bool getPoped();
	private:
		T val_;
		std::vector<MNode<T>*> children;
		MNode<T>* parent;
		size_t visitIndex;
		bool braceless;
		bool poped;
		size_t number;
	};
	//----< mark parent node as braceless node >----------------------------------------------------

	template <typename T>
	MNode<T>* MNode<T>::markParent()
	{
		this->parent->setBraceless();
		return this->parent;
	}
	//----< find current node's parent node>----------------------------------------------------

	template <typename T>
	MNode<T>* MNode<T>::getParent()
	{
		return this->parent;
	}

	//----< find current node's parent node and back to parent node>----------------------------------------------------

	template <typename T>
	MNode<T>* MNode<T>::findParent()
	{
		if (this->poped)
			this->parent->number = this->parent->number + 1;
		else
			this->parent->number = this->parent->number + number;
		this->poped = true;
		return this->parent;
	}
	//----< get poped bool >----------------------------------------------------

	template <typename T>
	bool MNode<T>::getPoped()
	{
		return poped;
	}

	//----< get Offspring number >----------------------------------------------------

	template <typename T>
	size_t MNode<T>::GetOffNum()
	{
		return number;
	}

	//----< return text >----------------------------------------------------

	template <typename T>
	T& MNode<T>::value() { return val_; }

	//----< promotion constructor >------------------------------------------

	template <typename T>
	MNode<T>::MNode(const T& t) : val_(t), visitIndex(0), number(1), braceless(false), poped(false){}

	//----< destructor >-------------------------------------------------------

	template <typename T>
	MNode<T>::~MNode()
	{
		//std::cout << "\n  deleting MNode";
		for (size_t i = 0; i < children.size(); ++i)
		{
			delete children[i];
		}
	}
	template <typename T>
	void MNode<T>::setBraceless()
	{
		braceless = true;
	}

	template <typename T>
	bool MNode<T>::getBraceless()
	{
		return braceless;
	}

	//----< return copy of self without children >---------------------------

	template <typename T>
	MNode<T>* MNode<T>::clone()
	{
		MNode<T>* pNode = new MNode<T>(value());
		for (size_t i = 0; i < children.size(); ++i)
			pNode->addChild(children[i]->clone());
		return pNode;
	}
	//----< copy constructor >-----------------------------------------------

	template <typename T>
	MNode<T>::MNode(const MNode<T>& node) : val_(node.val_)
	{
		for (size_t i = 0; i < node.children.size(); ++i)
			children.push_back(node.children[i]->clone());
		number = node.number;
	}
	//----< assignment operator >----------------------------------------------

	template <typename T>
	MNode<T>& MNode<T>::operator =(const MNode<T>& el)
	{
		if (&el == this) return *this;
		for (size_t i = 0; i < children.size(); ++i)
			delete children[i];
		children.clear();
		for (size_t i = 0; i < el.children.size(); ++i)
			addChild(el.children[i]->clone());
		return *this;
	}
	//----< add child node >---------------------------------------------------

	template <typename T>
	void MNode<T>::addChild(MNode<T>* pMNode)
	{
		if (pMNode == this)
			throw std::exception("attempting to make node a child of itself!");
		children.push_back(pMNode);
		////////////////////////////////
		// every child need record its parent
		pMNode->parent = this;
	}
	//----< return number of children >----------------------------------------

	template <typename T>
	size_t MNode<T>::numChildren() { return children.size(); }

	//----< remove child node - returns false on failure, else true >----------
	
	template <typename T>
	bool MNode<T>::removeChild(MNode<T>* pMNode)
	{
		std::vector< MNode<T>* >::iterator iter = children.begin();
		for (iter = children.begin(); iter != children.end(); ++iter)
		{
			if (pMNode == *iter)
			{
				delete pMNode;
				children.erase(iter);
				return true;
			}
		}
		return false;
	}
	//----< return pointer to next unvisited child or null >-----------------

	template <typename T>
	MNode<T>* MNode<T>::nextUnmarkedChild()
	{
		if (children.size() == 0)
			return 0;
		if (visitIndex < children.size())
		{
			return children[visitIndex++];
		}
		return 0;
	}
	//----< "mark" all children as unvisited >-------------------------------

	template <typename T>
	void MNode<T>::clearMarks()
	{
		visitIndex = 0;
	}
	//----< serialize content to string representation of node >-------------

	template <typename T>
	std::string MNode<T>::ToString()
	{
		std::ostringstream temp;
		temp << "<" << value() << ">";
		return temp.str();
	}

}
#endif
