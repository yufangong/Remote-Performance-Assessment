#ifndef TEXTSEARCH_H
#define TEXTSEARCH_H
///////////////////////////////////////////////////////////////////////////////
// TextSearch.h -   Implement text search function                           //
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
* This package contains one class: TextSearch. This class used to implement text search in Server
* Repository(default now) directory.
*
* Public Interface:
* =================
* std::string StartTextSearch(int threadNum, std::string keyword, CommandLine cl);
*
* Build Command:
* ==============
* devenv SocketDemo2.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 : 29 Apr 14
* - first release
*/

#include "../BlockingQueue/BlockingQueue.h"
#include "FileMger.h"

///////////////////////////////////////////////////////
// TextSearch

class TextSearch
{
public:
	using File = FileSystem::File;
	std::string StartTextSearch(int threadNum, std::string keyword, CommandLine cl);

private:
	void StartThreads(int threadNum, std::string keyword);
	void textSearch(std::string keyword);
	void PostFile(std::string filename);
	std::string GetFile();
	size_t OutPutQsize(){ return OutQ.size(); }
	BlockingQueue<std::string> InQ;
	BlockingQueue<std::string> OutQ;
	File* file;
	FileMger fm;
	std::string Result;
};
#endif
