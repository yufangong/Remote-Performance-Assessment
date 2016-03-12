/////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp - Search files with specific patterns and options           //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.1													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#include "FileMger.h"
#include <iostream>

//----< search for specific files >-------------------

Files FileMger::FindFiles(CommandLine cl_)
{
	path = std::get<0>(cl_);
	patts = std::get<1>(cl_);
	options = std::get<2>(cl_);
	bool recursive = false;
	for (auto opt : options)
	{
		if (opt == "/s" || opt == "/S")
			recursive = true;
	}
	path = "../Server/Repository";
	files = search(path, patts, recursive);
	return files;
}


//----< search for specific files >-------------------

Files& FileMger::search(const Path& path, Patterns patts, bool recursive)
{
	for (auto patt : patts)
	{
		Files temp = FileSystem::Directory::getFiles(path, patt);
		for (auto file : temp)
		{
			file = FileSystem::Path::fileSpec(path, file);
			files.push_back(file);
		}
	}
	// analyze options of command line to decide recursion

	if (recursive == true)
	{
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
	}
	return files;
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
