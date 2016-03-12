

/////////////////////////////////////////////////////////////////////////////
// Element.h - Element structure used to save information                  //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
*
* Package Operations:
* ===================
* This package contains one struct: element. This package use to let all relevent 
* module to use element.
*
* Public Interface:
* =================
* struct element
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

#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <sstream>
#include <iomanip>

struct element
{
	std::string type;
	std::string name;
	std::string file;
	size_t startline;
	size_t endline;
	size_t complexity;
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << startline;
		temp << ", ";
		temp << endline;
		temp << ", ";
		temp << complexity;
		temp << ")";
		return temp.str();
	}
};

#endif