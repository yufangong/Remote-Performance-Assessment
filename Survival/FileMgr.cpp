/////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp - Search files with specific patterns and options           //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include <iostream>

//----< search for specific files >-------------------

FileMgr::Files& FileMgr::search(const ComLineParsing::Path& path, ComLineParsing::Patterns patts, bool recursive)
{
	for (auto patt : patts)
	{
		Files temp = FileSystem::Directory::getFiles(path, patt);
		for (auto file : temp)
		{
			file = FileSystem::Path::fileSpec(path, file);
			_files.push_back(file);
		}
	}
	// analyze options of command line to decide recursion

	if (recursive == true)
	{
		//FileSystem::Directory::setCurrentDirectory(path);
		//std::string curdir = FileSystem::Directory::getCurrentDirectory();
		Paths dirs = FileSystem::Directory::getDirectories(path, "*.*");
		for (size_t i = 0; i < dirs.size(); i++)
		{
			if (dirs[i] != "." &&  dirs[i] != "..")
			{
				std::string subdir = dirs[i];
				std::string fullpath = path + "\\" + subdir;
				search(fullpath, patts, recursive);
			}
		}
		//FileSystem::Directory::setCurrentDirectory("..");
	}
	return _files;
}
#ifdef TEST_FILEMGR

int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; ++i)
		std::cout << "\n  " << argv[i];
	std::vector<std::string> foundFiles;
	std::vector<std::string> patterns;
	patterns.push_back("*.cpp");
	FileMgr fm;
	foundFiles = fm.search(".", patterns, true);
	for (auto file : foundFiles)
		std::cout << "\n  " << file;
	std::cout << "\n\n";
}
#endif
