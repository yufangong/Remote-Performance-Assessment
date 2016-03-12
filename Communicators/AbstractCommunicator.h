#ifndef ABSTRACTCOMMUNICATORS_H
#define ABSTRACTCOMMUNICATORS_H
//////////////////////////////////////////////////////////////////////////////
// AbstractCommunicators.h - Abstract base for message-passing communicator //
//                                                                          //
// -------------------------------------------------------------------------//
// Version:      1.1													    //   
// Language:     Visual C++, Visual Studio 2013                             //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional   //
// Application:  Spring Projects, 2014                                      //
// student:      Yufan Gong, Syracuse University                            //
//               (315) 247-3777, ygong01@syr.edu                            //
////////////////////////////////////////////////////////////////////////////// 
/*
*  Package Operations:
*  -------------------
*  This package:
*  AbstractCommunicators
*    Serves as a base class for named objects that process messages on child threads.

*
*  Public Interface:
*  =================
*  Start: create thread
*  ProcessMsg: process messages in the queue, dispatch them
*  SetName, GetName.
*
*  Required Files:
*  - BlockingQueue.h          Defines queue process
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

#include "../BlockingQueue/BlockingQueue.h"
#include <thread>
#include <map>
#include "../Display/Display.h"
#include "../Survival/Executive.h"
#include "../TextSearch/TextSearch.h"

///////////////////////////////////////////////////////////////////////
// AbstractCommunicators
// - defines parts that accept messages
// - each Communicator has a thread for processing messages
// - behavior is defined by application override of ProcessMsg()

class AbstractCommunicators 
{
public:
	virtual void PostMsg(ServiceMessage message);
	void Start();
	void SetName(std::string name){ Name = name; }
	std::string GetName(){ return Name; }
protected:
	ServiceMessage GetMsg();
	virtual void ProcessMsg() = 0;
	BlockingQueue<ServiceMessage> bq;
	std::string Name;
	Display *dp;
};

//----< post message in abstract class's blocking queue>----------------

inline void AbstractCommunicators::PostMsg(ServiceMessage message)
{
	bq.enQ(message);
}

//----< get message from abstract class's blocking queue>----------------

inline ServiceMessage AbstractCommunicators::GetMsg()
{
	return bq.deQ();
}

//----< create a thread to process messages >----------------

inline void AbstractCommunicators::Start()
{
	std::thread t1(&AbstractCommunicators::ProcessMsg, this);
	t1.detach();
}


#endif   