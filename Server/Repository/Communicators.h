#ifndef COMMUNICATORS_H
#define COMMUNICATORS_H
/////////////////////////////////////////////////////////////////////////////
// Communicators.h - Abstract base for message-passing communicator        //
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
*  Package Operations:
*  -------------------
*  This package contains three classes:
*  AbstractCommunicators
*    Serves as a base class for named objects that process messages on child threads.
*  EchoCommunicator
*    This communicator implement process text messages from another peer, just 
*    recieve messages and maybe reply.
*  FileCommunicator
*    This concrete communicator response to handle file transfer function, it should 
*    recieve files by blocks and send files by blocks
*
*  Public Interface:
*  =================
*  Start: create thread
*  ProcessMsg: process messages in the queue, dispatch them
*  SetName, GetName.
*
*  Required Files:
*  - AbstractCommunicator.h   Defines Communicator behavior
*  - BlockingQueue.h          Defines queue process
*
*
*  Build Command:   cl /EHa /DTEST_COMMUNICATORS communicators.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : Apr 12, 2014
*  - first release
*/

#include "AbstractCommunicator.h"
//why this class cannot as aggregation in other classes

class HelpClass
{
public:
	CommandLine ParseCondition(std::string condition);
};

///////////////////////////////////////////////////////////////////////
// EchoCommunicator 

class EchoCommunicator : public AbstractCommunicators
{
public:
	void getDispatcher(AbstractCommunicators* dispatcher);
private:
	void ProcessMsg();
	CommandLine cl;
	HelpClass hp;
	AbstractCommunicators* dispatcher_;
};

///////////////////////////////////////////////////////////////////////
// FileCommunicator 

class FileCommunicator : public AbstractCommunicators
{
public:
	using File = FileSystem::File;
	void getDispatcher(AbstractCommunicators* dispatcher);
private:
	void ProcessMsg();
	void RecieveFile(ServiceMessage message);
	void SendFile(ServiceMessage message);
	AbstractCommunicators* dispatcher_;
	std::map<std::string, File*> fileBuffer;
	std::map<std::string, File*>::iterator fileIter;
};



///////////////////////////////////////////////////////////////////////
// SimilarityCommunicator 

using Patterns = std::vector<std::string>;
using Path = std::string;
using Options = std::vector<std::string>;
using CommandLine = std::tuple<Path, Patterns, Options>;

class SimilarityCommunicator : public AbstractCommunicators
{
public: 
	void getDispatcher(AbstractCommunicators* dispatcher);
private:
	void ProcessMsg();
	//CommandLine ParseCondition(std::string condition);
	AbstractCommunicators* dispatcher_;
	CommandLine cl;
	IExec* pIExec;
	HelpClass hp;
};

///////////////////////////////////////////////////////////////////////
// SearchCommunicator
class SearchCommunicator : public AbstractCommunicators
{
public:
	void getDispatcher(AbstractCommunicators* dispatcher);
private:
	void ProcessMsg();
	AbstractCommunicators* dispatcher_;
	CommandLine cl;
	HelpClass hp;
};



#endif   