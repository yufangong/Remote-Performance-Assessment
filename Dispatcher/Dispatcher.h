#ifndef DISPATCHER_H
#define DISPATHCER_H
/////////////////////////////////////////////////////////////////////////////
// Dispatcher.h - dispatch messages to concrete communicators              //
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
*
* Module Operations:
* ==================
* This package provide one class to dispatch messages to concrete communicators
* each communicators registered in dispatcher will stored in a map and recieve 
* messages
*
* Public Interface:
* =================
* Register: store communicators in map
* ProcessMsg: process messages in the queue, dispatch them
*
* Build Command:   cl /EHa /DTEST_DISPATCHER dispatcher.cpp
* ===========================================================
*
*  Required Files:
*  - sender.h   create sender if need to reply
*  - BlockingQueue.h          Defines queue process
*
*
* Maintenance History:
* ====================
* ver 1.0 : 12 Mar 14
* - first release
*
*/
#include "../Sender/Sender.h"

///////////////////////////////////////////////////////////////////////
// CommunicatorDispatcher
// - dispatch messages

class CommunicatorDispatcher : public AbstractCommunicators
{
public:
	void Register(AbstractCommunicators* communicator);
	void ProcessMsg();
	//void SetVerbose();
private:
	std::map<std::string, AbstractCommunicators*> CommunicatorLookup;
	std::map<std::string, AbstractCommunicators*>::iterator mapIter;
	//bool verbose;
};
#endif
