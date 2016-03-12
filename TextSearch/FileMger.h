

#ifndef FILEMGER_H
#define FILEMGER_H
/////////////////////////////////////////////////////////////////////////////
// FileMgr.h - Search files with specific patterns and options             //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.1													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

/*
* Module Operations:
* ==================
* This package provide a method to find files with specific patterns and options
* in specific paths. 
* it provides a recursive method to search files in all subdirectories.
* 
* Public Interface:
* =================
* std::vector<files>& search(paths,patterns,recursive);
* Return search results files.
*
* Maintenance History:
* ====================
* ver 1.1 : 29 Apr 14
* - fix reusable problem
* ver 1.0 : 15 Feb 14
* - first release
*
*/

#include <vector>
#include <string>
#include <tuple>
#include "../FileSystem/FileSystem.h"

/////////////////////////////////////////////////////////////////////////
// FileMgr class

using Patterns = std::vector<std::string>;
using Path = std::string;
using Options = std::vector<std::string>;
using CommandLine = std::tuple<Path, Patterns, Options>;
using Files = std::vector<std::string>;
using Paths = std::vector<std::string>;

class FileMger
{
public:
	Files FindFiles(CommandLine cl_);
	Files& search(const Path& path, Patterns patts, bool recursive);
private:
	Path path;
	Patterns patts;
	Options options;
	Files files;
	Patterns _patterns;
};
#endif
