
#ifndef SCOPESTACK_H
#define SCOPESTACK_H
/////////////////////////////////////////////////////////////////////////////
// ScopeStack.h - implements template stack holding specified element type //
//                create nodes when elements pushed in stack               //
// ver 3.0                                                                 //
// Language:      Visual C++, Visual Studio 2013                           //
// platform:      Macbook pro, Parallels Desktop, Windows 7 profession     //
// Application:   spring 2014 Projects                                     //
// Author:        Jim Fawcett, CST 4-187, Syracuse University              //
//                (315) 443-3948, jfawcett@twcny.rr.com                    //
/////////////////////////////////////////////////////////////////////////////
/*

Package Operations:
===================
This package contains one class: ScopeStack<element>.  The element type
is an application specific type designed to hold whatever information you
need to stack.
When push into stack an element which is specific, create nodes and build trees.

Public Interface:
=================
ScopeStack<element> stack;
// define some element type
element elem;
stack.push(elem);
element popped = stack.pop();
size_t complexity = popComplex()

Required Files:
===============
ScopeStack.h, ScopeStack.cpp

Build Command:
==============
devenv ScopeStack.sln /rebuild debug

Maintenance History:
====================
ver 3,0 : 21 Mar 14
- back to ver 2.1

ver 2.2 : 02 Feb 14
- add Nodes part, each time push in specific element can add nodes to 
an exist node or create a new root node; each time pop out element current 
node will back to its parent node;

ver 2.1 : 02 Jun 11
- changed ScopeStack type to single template argument intended to be
an application specific type (usually a struct that holds whatever
is needed)
ver 2.0 : 02 Feb 11
- fixed bug in pop() method found by Drumil Bhattad
ver 1.0 : 31 Jan 11
- first release
*/


#include <list>
#include <vector>

template<typename element>
class ScopeStack
{
public:
	typename typedef std::list<element>::iterator iterator;
	void push(const element& item);
	element pop();
	size_t size();
	iterator begin();
	iterator end();
private:
	std::list<element> stack;
};

template<typename element>
typename inline size_t ScopeStack<element>::size() { return stack.size(); }

template<typename element>
typename inline ScopeStack<element>::iterator ScopeStack<element>::begin() { return stack.begin(); }

template<typename element>
typename inline ScopeStack<element>::iterator ScopeStack<element>::end() { return stack.end(); }

template<typename element>

//----< push element into stack >-----------
void ScopeStack<element>::push(const element& item)
{
	stack.push_back(item);
	
}

//----< pop element out of stack >-----------

template<typename element>
element ScopeStack<element>::pop()
{
	element item = stack.back();
	stack.pop_back();
	return item;
}


//----< show stack >-----------

template<typename element>
void showStack(ScopeStack<element>& stack, bool indent = true)
{
	if (stack.size() == 0)
	{
		std::cout << "\n  ScopeStack is empty";
		return;
	}
	ScopeStack<element>::iterator iter = stack.begin();
	while (iter != stack.end())
	{
		std::string strIndent = std::string(2 * stack.size(), ' ');
		if (!indent)
			strIndent = "";
		std::cout << "\n  " << strIndent << iter->show();
		++iter;
	}
}
#endif
