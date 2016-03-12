

/////////////////////////////////////////////////////////////////////////////
// Display.h -  Display package used to print output to the screen.        //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      2.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////


/*
* Module Operations:
* ==================
* This package used to print parse results to the screen.
*
* Public Interface:
* =================
* void showCommandLineParse(CommandLine cl);
* void Demonstration(size_t require);
* void showResults(std::map<Node*, std::vector<Node*>>&, std::vector<std::string> Files, bool difference);
*
* 
* Maintenance History:
* ====================
* ver 2.0 : 21 Mar 14
* - change to display similarity analysis results
*
* ver 1.0 : 14 Feb 14
* - first release
*/


#ifndef DISPLAY_H
#define DISPLAY_H

#include "Element.h"
#include <iostream>
#include <iomanip>
#include "ComLineParsing.h"
#include <map>
#include "MNode.h"
#include <sstream>

class Display
{
public:
	using Node = TMTree::MNode<element>;
	using Patterns = std::vector<std::string>;
	using Path = std::string;
	using Options = std::vector<std::string>;
	using CommandLine = std::tuple<Path, Patterns, Options>;
	void showCommandLineParse(CommandLine cl);
	void Demonstration(size_t require);
	void showResults(std::map<Node*, std::vector<Node*>>&, std::vector<std::string> Files, bool difference);
	std::string GetResults(std::map<Node*, std::vector<Node*>>&, std::vector<std::string> Files, bool difference);

private:
	void showTwoFilesSame(std::map<Node*, std::vector<Node*>>& Results, bool difference);
	void showSimiInTwoFile(std::map<Node*, std::vector<Node*>>& Results, std::vector<std::string> Files, bool difference);
	void showSimiInOneFile(std::map<Node*, std::vector<Node*>>& Results, bool difference);
	void similarRegionCount(std::map<Node*, std::vector<Node*>>& Results);
	void showDifference(Node* node1, Node* node2);

	std::string GetTwoFilesSame(std::map<Node*, std::vector<Node*>>& Results, bool difference);
	std::string GetSimiInTwoFile(std::map<Node*, std::vector<Node*>>& Results, std::vector<std::string> Files, bool difference);
	std::string GetSimiInOneFile(std::map<Node*, std::vector<Node*>>& Results, bool difference);
	std::string GetsimilarRegionCount(std::map<Node*, std::vector<Node*>>& Results);
	std::string GetDifference(Node* node1, Node* node2);
};
#endif

