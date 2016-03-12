#ifndef RECIEVER_H
#define RECIEVER_H
/////////////////////////////////////////////////////////////////////////////
// Reciever.h - Demonstration of socket reciever with concurrent clients   //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.2													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This package provide three classes:
*   ClientHandlerThread : readlines and recieve messages content
*   ListenThread : listen port to create new sockets object handle different port client
*   Reciever : recieve messages and put them into dispatcher queue
*
*  blocksize   : 1024(defined in client side)
*  where 1024 is a stand-in for whatever you want your block
*  size to be.
*
*
* Public Interface:
* =================
* getDispatcher: get dispatcher pointer
* start: start main thread and create sub threads
*
*
* Build Process:
* --------------
* cl /EHa /DTEST_RECIEVER Reciever.h
*
* Required files:
* - Reciever.cpp, Sockets.h, Sockets.cpp,
*   Threads.h, Threads.cpp, Locks.h, Locks.cpp
*   BlockingQueue.h, BlockingQueue.cpp
*
*
* Maintenance History:
* ====================
* ver 1.2 : 12 Apr 2014
* - update to c++11 thread 
* - changed to recieve my http style messages
*
* ver 1.1 - 30 Mar 2013
* - changes to error handling
* ver 1.0 - 29 Mar 2013
* - first release
*
*/
#include "../Sockets/Sockets.h"
#include "../BlockingQueue/BlockingQueue.h"
#include <string>
#include "../Dispatcher/Dispatcher.h"
///////////////////////////////////////////////////
// ClientHandlerThread thread

class ClientHandlerThread
{
public:
	ClientHandlerThread(Socket s, BlockingQueue<ServiceMessage>& q) : s_(s), q_(q) {}

	//----< recieve messages, rebuild them into messages >----------------
	void run()
	{

		std::string str;
		int count;
		ServiceMessage message;

		do
		{
			count = 0;
			do
			{
				str = s_.readLine();
				if (str == "\n")
					break;
				message.rebuildMsgHeader(str, count);
				count++;
			} while (true);
			const int length = message.GetLength() + 2;
			char* block = new char[length];
			s_.recvAll(block, length);
			message.rebuildMsgBody(block);
			q_.enQ(message);
		} while (message.GetCommand() != "Stop");
		s_.disconnect();
	}
private:
	Socket s_;
	BlockingQueue<ServiceMessage>& q_;
};

///////////////////////////////////////////////////
// listenThread thread
class ListenThread 
{
public:
	ListenThread(int port, BlockingQueue<ServiceMessage>& q) : listener_(port), q_(q), stop_(false) {}
	void stop(bool end) { stop_ = end; }

	//----< listen port, create new clienthandler thread if new port connect to it >----------------
	void run()
	{

		while (!stop_)
		{
			SOCKET s = listener_.waitForConnect();
			ClientHandlerThread* pCh = new ClientHandlerThread(s, q_);
			std::thread tpCh(&ClientHandlerThread::run, pCh);
			tpCh.detach();
		}
	}
private:
	bool stop_;
	BlockingQueue<ServiceMessage>& q_;
	SocketListener listener_;
};

///////////////////////////////////////////////////
//  Receiver
class Receiver
{
public:

	//----< get dispatcher pointer >----------------

	void getDispatcher(CommunicatorDispatcher* dispatcher)
	{
		dispatcher_ = dispatcher;
	}

	//----< start main thread and create sub threads >----------------
	void Start(int port)
	{
		std::thread t(&Receiver::processMsg, this, port);
		t.detach();
	}
	void processMsg(int port)
	{
		std::cout << "\n Reciever started, Port: " << port << std::endl;
		pLt = new ListenThread(port, q_);
		std::thread tpLt(&ListenThread::run, pLt);

		try
		{
			tpLt.detach();
			ServiceMessage msg;

			while (true)
			{
				msg = q_.deQ();
				dispatcher_->PostMsg(msg);
			}
			//pLt->stop(true);
			//do
			//{
			//	msg = q_.deQ();
			//	dispatcher_->PostMsg(msg);
			//} while (msg.GetCommand()!= "Stop");
		}
		catch (std::exception)
		{
			delete pLt;
		}
		catch (...)
		{
			delete pLt;
		}
		//delete pLt;

	}

private:
	BlockingQueue<ServiceMessage> q_;
	ListenThread* pLt;
	CommunicatorDispatcher* dispatcher_;
};
#endif

#ifdef TEST_RECIEVER
//----< test stub >----------------

int main()
{
	CommunicatorDispatcher* dispatcher = new CommunicatorDispatcher();
	dispatcher->SetName("dispatcher");

	Receiver* rcvr = new Receiver();
	rcvr->getDispatcher(dispatcher);
	std::thread tps(&Receiver::start, rcvr, 8080);

	Sender* ps = new Sender();
	ps->SetName("sender");
	ps->Connect("127.0.0.1", 8080);
	ps->Start();

	std::string ServerIp = "127.0.0.1";
	int ServerPort = 8080;
	std::string LocalIp = "127.0.0.1";
	int LocalPort = 9000;
	ServiceMessage echoMessage;
	echoMessage.makeHeader("Echo", ServerIp, ServerPort, LocalIp, LocalPort, "echo", "textMsg");
	echoMessage.makeContent("This is a text message.");

	ps->PostMsg(echoMessage);
	while (true);
}
#endif


