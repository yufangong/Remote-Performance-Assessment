///////////////////////////////////////////////////////////////////////////////
// ScopeStack.cpp - implements template stack holding specified element type //
//                - implement store trees root node                          //
// ver 3.0                                                                   //
// Language:      Visual C++, Visual Studio 2013                             //
// platform:      Macbook pro, Parallels Desktop, Windows 7 profession       //
// Application:   spring 2014 Projects                                       //
// Author:        Jim Fawcett, CST 4-187, Syracuse University                //
//                (315) 443-3948, jfawcett@twcny.rr.com                      //
// Modified:      Yufan Gong, Syracuse University                            //
//                (315) 247-3777, ygong01@syr.edu                            //
///////////////////////////////////////////////////////////////////////////////

#include "ScopeStack.h"
#include "Display.h"

//----< test stub >------------------------------------------------------

#ifdef TEST_SCOPESTACK

#include "ScopeStack.h"
#include <string>
#include <iostream>
#include <sstream>

using stack = ScopeStack<element>;

int main()
{
	std::cout << "\n  Testing Scope Stack";
	std::cout << "\n =====================\n";
	std::cout << "\n  pushing items onto ScopeStack instance";
	std::cout << "\n ----------------------------------------";
	stack testStack;
	element item;
	item.type = "unknown";
	item.name = "fun3";
	item.startline = 73;
	testStack.push(item);
	testStack.pop();
	item.type = "function";
	item.name = "fun1";
	item.startline = 33;
	testStack.push(item);
	item.type = "if";
	item.name = "fun2";
	item.startline = 53;
	testStack.push(item);
	showStack<element>(testStack, false);
	std::cout << std::endl;
	std::cout << "\n  Popping items off ScopeStack";
	std::cout << "\n ----------------------------------";
	testStack.pop();
	testStack.pop();
	showStack<element>(testStack, false);
}

#endif
