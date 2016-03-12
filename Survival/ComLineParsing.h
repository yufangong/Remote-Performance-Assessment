/////////////////////////////////////////////////////////////////////////////
// CommandLineParsing.h - Parsing commandline and extract information      //
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
* Module Operations:
* ==================
* This package parses command line user input;
*
* Public Interface:
* =================
* IsOption: is option or not
* parseCommandLine: parsing command line and store information in a tuple
*
* Maintenance History:
* ====================
* ver 1.0 : 21 Mar 14
* - first release
*
*/


#ifndef COMLINEPARSING_H
#define COMLINEPARSING_H

#include <vector>
#include <string>
#include <tuple>

class ComLineParsing
{
public:
	ComLineParsing();
	~ComLineParsing();
	using Patterns = std::vector<std::string>;
	using Path = std::string;
	using Options = std::vector<std::string>;
	using CommandLine = std::tuple<Path, Patterns, Options>;
	CommandLine parseCommandLine(int argc, char* argv[]);
private:
	bool IsOption(const std::string& token);
};
#endif