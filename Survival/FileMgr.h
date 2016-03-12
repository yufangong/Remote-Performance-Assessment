

#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////////////
// FileMgr.h - Search files with specific patterns and options             //
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
* ver 1.0 : 15 Feb 14
* -first release
*
*/

#include <vector>
#include <string>
#include "ComLineParsing.h"
#include "FileSystem.h"

/////////////////////////////////////////////////////////////////////////
// FileMgr class
class FileMgr
{
public:
	using Files = std::vector<std::string>;
	using Paths = std::vector<std::string>;
	Files& search(const ComLineParsing::Path& path, ComLineParsing::Patterns patts, bool recursive);
private:
	ComLineParsing::Path path;
	Files _files;
	ComLineParsing::Patterns _patterns;
};
#endif
