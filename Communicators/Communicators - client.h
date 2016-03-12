#ifndef COMMUNICATORSCLIENT_H
#define COMMUNICATORSCLIENT_H
/////////////////////////////////////////////////////////////////////////////
// Communicators-client.h - base classes for message-passing communicator  //
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
*  Package Operations:
*  -------------------
*  This package contains two classes:
*
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
*
*
*  Build Command:   cl /EHa /DTEST_COMMUNICATORS communicators.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : Apr 29, 2014
*  - apart abstractcommunicator and its derieved classes
*  ver 1.0 : Apr 12, 2014
*  - first release
*/

#include "AbstractCommunicator.h"

///////////////////////////////////////////////////////////////////////
// EchoCommunicator 

class EchoCommunicator : public AbstractCommunicators
{
public:
	EchoCommunicator(BlockingQueue<ServiceMessage>& outQ) :outputQ_(outQ){}
private:
	void ProcessMsg();
	CommandLine cl;
	FileMger fm;
	AbstractCommunicators* dispatcher_;
	BlockingQueue<ServiceMessage>& outputQ_;
};

///////////////////////////////////////////////////////////////////////
// FileCommunicator 

class FileCommunicator : public AbstractCommunicators
{
public:
	using File = FileSystem::File;
	FileCommunicator(BlockingQueue<ServiceMessage>& outQ) :outputQ_(outQ){}
private:
	void ProcessMsg();
	void RecieveFile(ServiceMessage message);
	void SendFile(ServiceMessage message);
	AbstractCommunicators* dispatcher_;
	std::map<std::string, File*> fileBuffer;
	std::map<std::string, File*>::iterator fileIter;
	BlockingQueue<ServiceMessage>& outputQ_;

};




#endif   