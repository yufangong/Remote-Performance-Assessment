#ifndef SENDER_H
#define SENDER_H
/////////////////////////////////////////////////////////////////////////////
// Sender.h - Demonstration of concurrent socket connectors                //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      3.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
*  This package provides one class: Sender
*  Sender class can send my http style messages to sockets
*  blocksize   : 1024(defined by designer)
*  where 1024 is a stand-in for whatever you want your block
*  size to be.
*
*
* Public Interface:
* =================
* Connect: connect to sockets
* SendFile: sendfiles to sockets
* Disconnect: disconnect to sockets
*
* Build Process:
* --------------
* cl /EHa /DTEST_SENDER Sender.h
*
* Required files:
* - Sender.cpp, Sockets.h, Sockets.cpp,
*   Threads.h, Threads.cpp, Locks.h, Locks.cpp
*   BlockingQueue.h, BlockingQueue.cpp
*
*
* Maintenance History:
* ====================
* ver 1.2 : 12 Apr 2014
* - update to c++11 thread
* - changed to send my http style messages
*
* ver 1.1 - 30 Mar 2013
* - changed Sendthread from terminating to default
* - minor changes to error handling
* ver 1.0 - 29 Mar 2013
* - first release
*
*/

#include "../sockets/Sockets.h"
#include "../BlockingQueue/BlockingQueue.h"
#include <string>
#include <iostream>
#include <sstream>
#include "../Communicators/AbstractCommunicator.h"
#include "../FileSystem/FileSystem.h"

///////////////////////////////////////////////////
// Sender
class Sender : public AbstractCommunicators
{
public:
	void Connect(std::string ip, int port);
	void SendFile(ServiceMessage message);
private:
	void ProcessMsg();
	Socket s_;
};
#endif