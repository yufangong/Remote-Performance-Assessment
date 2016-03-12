

/////////////////////////////////////////////////////////////////////////////
// Executive.h -  The console of whole program, sequence the executive     //
//                action                                                   //
// ver 1.0                                                                 //
// Language:      Visual C++, Visual Studio 2013                           //
// platform:      Macbook pro, Parallels Desktop, Windows 7 profession     //
// Application:   spring 2014 Projects                                     //
// student:       Yufan Gong, Syracuse University                          //
//                (315) 247-3777, ygong01@syr.edu                          //
/////////////////////////////////////////////////////////////////////////////
/*
*
* Package Operations:
* ===================
* This package contains one class: collection. This class used to push nodes into
* shared node repository build trees
*
* Public Interface:
* =================
* int parseFiles(const std::vector<std::string>& myFiles, bool braceless);
* void Analysis(int argc, char* argv[]);
* void showResult();
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


#include <vector>

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

struct IExec
{
	using Patterns = std::vector<std::string>;
	using Path = std::string;
	using Options = std::vector<std::string>;
	using CommandLine = std::tuple<Path, Patterns, Options>;


	virtual DLL_DECL ~IExec() {}

	static DLL_DECL IExec* Create();
	virtual DLL_DECL void Analysis(CommandLine cl_) = 0;
	virtual DLL_DECL std::string getResultToString() = 0;

};