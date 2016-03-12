/////////////////////////////////////////////////////////////////////////////
// Executive.cpp -  Start and console of this project                      //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#define IN_DLL

#include "Executive.h"
#include "FileMgr.h"
#include "ComLineParsing.h"
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include <string>
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FoldingRules.h"
#include "ChronoTimer.h"
#include "SimiAnalysis.h"

//----< constructor to initialize data member >-------------------
class Executive : public IExec
{
public:
	Executive();
	~Executive();
	int parseFiles(const std::vector<std::string>& myFiles, bool braceless);
	void Analysis(CommandLine cl_);
	void Analysis(int argc, char* argv[]);
	std::map<Node*, std::vector<Node*>>& getResults();
	void showResult();
	std::string getResultToString();
private:
	ConfigParseToConsole configure;
	CommandLine cl;
	ComLineParsing clp;
	Path path;
	Patterns patts;
	Options options;
	FileMgr fm;
	FileMgr::Files myFiles;
	SimiAnalysis simi;
	Display dp;
	bool recursive;
	bool difference;
	bool braceless;
	bool repeatless;
};
IExec* IExec::Create(){ return new Executive(); }

Executive::Executive()
{
	recursive = false;
	difference = false;
	braceless = false;
	repeatless = false;
}
//----< destructor >-------------------

Executive::~Executive()
{

}
void Executive::Analysis(CommandLine cl_)
{
	path = std::get<0>(cl_);
	patts = std::get<1>(cl_);
	options = std::get<2>(cl_);
	for (auto opt : options)
	{
		if (opt == "/s" || opt == "/S")
			recursive = true;
		if (opt == "/d" || opt == "/D")
			difference = true;
		if (opt == "/b" || opt == "/B")
			braceless = true;
		if (opt == "/r" || opt == "/R")
			repeatless = true;
	}
	FileMgr fm;
	myFiles = fm.search(path, patts, recursive);
	parseFiles(myFiles, braceless);
	simi.SortRepository();
	simi.ComplexityCompare(repeatless);
}
//----< console analysis procedure >-------------------

void Executive::Analysis(int argc, char* argv[])
{
	cl = clp.parseCommandLine(argc, argv);
	path = std::get<0>(cl);
	patts = std::get<1>(cl);
	options = std::get<2>(cl);
	for (auto opt : options)
	{
		if (opt == "/s" || opt == "/S")
			recursive = true;
		if (opt == "/d" || opt == "/D")
			difference = true;
		if (opt == "/b" || opt == "/B")
			braceless = true;
		if (opt == "/r" || opt == "/R")
			repeatless = true;
	}
	myFiles = fm.search(path, patts, recursive);
	parseFiles(myFiles, braceless);
	simi.SortRepository();
	simi.ComplexityCompare(repeatless);
}
//----< used to parse each file >-------------------

int Executive::parseFiles(const std::vector<std::string>& myFiles, bool braceless)
{
	for (auto file : myFiles)
	{
		Parser* pParser = configure.Build(file, braceless);
		try
		{
			if (pParser)
			{
				if (!configure.Attach(file))
				{
					std::cout << "\n  could not open file " << file << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}
			while (pParser->next())
				pParser->parse();
			if (braceless)
				configure.pRepo1->collection().setFiles();
			else
				configure.pRepo2->collection().setFiles();
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
	return 0;
}
//----< used to get analysis result >-------------------
std::string Executive::getResultToString()
{
	std::string results;
	std::map<Node*, std::vector<Node*>>& Results = simi.getResults();
	results = dp.GetResults(Results, myFiles, difference);
	return results;
}

//----< used to show analysis result >-------------------

void Executive::showResult()
{
	dp.showCommandLineParse(cl);
	if (recursive)
		dp.Demonstration(3);
	if (difference)
		dp.Demonstration(6);
	if (braceless)
		dp.Demonstration(5);
	if (repeatless)
		dp.Demonstration(7);
	else
		dp.Demonstration(4);
	std::map<Node*, std::vector<Node*>>& Results = simi.getResults();
	dp.showResults(Results, myFiles, difference);
}
//----< start program >-------------------

int main(int argc, char* argv[])
{
	Timer timer;
	if (argc < 3)
	{
		std::cout << "\n  please enter path and patterns\n\n";
		return 1;
	}
	timer.start();
	Executive exe;
	exe.Analysis(argc, argv);
	timer.stop();
	std::cout << "\n *************************************";
	std::cout << "\n measured time in milliseconds = " << timer.elapsedTime<std::chrono::milliseconds>();
	std::cout << "\n *************************************\n";
	exe.showResult();
	std::cout << "\n\n";
	return 0;
}