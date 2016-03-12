/////////////////////////////////////////////////////////////////////////////
// CommandLineParsing.cpp - Parsing commandline and extract information    //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#include "ComLineParsing.h"
#include <iostream>
#include "FileSystem.h"


ComLineParsing::ComLineParsing()
{
}


ComLineParsing::~ComLineParsing()
{
}

//----< helper function - is this token an option? >-----------------------

bool ComLineParsing::IsOption(const std::string& token)
{
	return (token[0] == '/');
}

//----< return tuple { path, patterns, options } from commandline >--------

ComLineParsing::CommandLine ComLineParsing::parseCommandLine(int argc, char* argv[])
{
	Options options;
	Patterns patterns;
	Path path;
	if (argc == 1)
		path = FileSystem::Path::getFullFileSpec(".");  // default path

	for (int i = 1; i < argc; ++i)
	{
		if (IsOption(argv[i]))
			options.push_back(argv[i]);
		else
		{
			if (path.size() == 0)
				path = FileSystem::Path::getFullFileSpec(argv[i]);
			else
				patterns.push_back(argv[i]);
		}
	}
	return CommandLine{ path, patterns, options };
}

#ifdef TEST_COMLINEPARSING
#include "Display.h"
int main(int argc, char*argv[])
{
	ComLineParsing clp;
	std::tuple<std::string, std::vector<std::string>, std::vector<std::string>> commands;
	commands = clp.parseCommandLine(argc, argv);
	Display dp;
	dp.showCommandLineParse(commands);
}
#endif

