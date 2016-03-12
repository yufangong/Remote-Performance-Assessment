///////////////////////////////////////////////////////////////////////////////
// TextSearch.cpp - Implement text search function                           //
// ver 1.0                                                                   //
// Language:        Visual C++, Visual Studio 2013                           //
// platform:        Macbook pro, Parallels Desktop, Windows 7 profession     //
// Application:     spring 2014 Projects                                     //
// student:         Yufan Gong, Syracuse University                          //
//                  (315) 247-3777, ygong01@syr.edu                          //
///////////////////////////////////////////////////////////////////////////////

#include "TextSearch.h"

//----< put filenames in queue and start text search threads >-------------------

std::string TextSearch::StartTextSearch(int threadNum, std::string keyword, CommandLine cl)
{
	Files files = fm.FindFiles(cl);
	for (auto file : files)
	{
		PostFile(file);
	}
	StartThreads(threadNum, keyword);
	if (OutPutQsize() == 0)
	{
		Result = "No items match your search.";
	}
	else
	{
		while (OutQ.size() > 0)
		{
			Result += GetFile();
			Result += "|";
		}
	}
	return Result;
}

//----< start threads >-------------------

void TextSearch::StartThreads(int threadNum, std::string keyword)
{
	std::vector<std::thread*> threadvector;
	for (int i = 0; i < threadNum; i++)
	{
		std::thread* t = new std::thread(&TextSearch::textSearch, this, keyword);
		threadvector.push_back(t);
		//std::thread([&]()->void{textSearch(keyword); }).detach();
		//threadvector.push_back(new std::thread([&]()->void{textSearch(keyword); }));
	}
	for (auto t : threadvector)
	{
		t->join();
	}
}

//----< each thread gets files and search in it, put result in outQ >-------------------

void TextSearch::textSearch(std::string keyword)
{
	while (InQ.size() > 0)
	{
		std::string filecontent;
		std::string filename = InQ.deQ();
		file = new File(filename);

		file->open(File::in);
		while (file->isGood())
		{
			filecontent += file->getLine();
		}
		size_t found = filecontent.find(keyword);
		if (found != std::string::npos)
		{
			OutQ.enQ(filename);
		}
	}
}

//----< put filename in pre-process queue >-------------------

void TextSearch::PostFile(std::string filename)
{ 
	InQ.enQ(filename); 
}
//----< get result from after-process queue >-------------------

std::string TextSearch::GetFile()
{
	return OutQ.deQ();
}

#ifdef TEST_TEXTSEARCH
//-------<test stub>------------
#include <iostream>
int main()
{
	TextSearch* ts = new TextSearch();
	ts->PostFile("TextSearch.cpp");
	ts->StartThreads(3,"text");
	std::string filenames;
	while (ts->OutPutQsize() > 0)
	{
		filenames += ts->GetFile();
	}
	std::cout << filenames;
}
#endif