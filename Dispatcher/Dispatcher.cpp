/////////////////////////////////////////////////////////////////////////////
// Dispatcher.cpp - dispatch messages to concrete communicators            //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#include "Dispatcher.h"

//----<register each concrete communicators into its map >--------

void CommunicatorDispatcher::Register(AbstractCommunicators* communicator)
{
	if (communicator->GetName() == this->GetName())  // don't allow MessageDispatcher to register itself
		return;
	CommunicatorLookup[communicator->GetName()] = communicator;
}

//----< set verbose >--------

//void CommunicatorDispatcher::SetVerbose()
//{
//	verbose = true;
//}

//----< process messages: put them into correspond communicators >--------

void CommunicatorDispatcher::ProcessMsg()
{

	while (true)
	{
		ServiceMessage msg = GetMsg();
		if (msg.GetCommand() == "reply" || msg.GetCommand() == "reply_download" || msg.GetCommand() == "reply_files" || msg.GetCommand() == "reply_simi" || msg.GetCommand() == "reply_search" || msg.GetCommand() == "reply_upload" || msg.GetPath() =="Stop")
		{
			mapIter = CommunicatorLookup.find(msg.IntToString(msg.GetPort()));
			if (mapIter != CommunicatorLookup.end())
			{
				mapIter->second->PostMsg(msg);  // send on to TargerSender
			}
			else
			{
				Sender* sndr = new Sender();
				sndr->Connect(msg.GetIp(), msg.GetPort());
				sndr->SetName(msg.IntToString(msg.GetPort()));
				Register(sndr);
				sndr->Start();
				sndr->PostMsg(msg);
			}
			//if (msg.GetCommand() == "reply")
				//dp->ShowMessage(5, msg);
		}
		else
		{
			mapIter = CommunicatorLookup.find(msg.GetTargetComm());
			if (mapIter != CommunicatorLookup.end())
			{
				mapIter->second->PostMsg(msg);  // send on to TargetCommunicator
			}
			else
			{
				std::cout << "Target communicator does not exist.";
			}
		}
	}
}
#ifdef TEST_DISPATCHER

//----< test stub>--------

int main()
{
	CommunicatorDispatcher* dis = new CommunicatorDispatcher();
	dis->SetName("dis");
	EchoCommunicator* echo = new EchoCommunicator();
	echo->SetName("echo");
	dis->Register(echo);

	FileCommunicator* file = new FileCommunicator();
	file->SetName("file");
	dis->Register(file);

	echo->Start();
	file->Start();
	dis->Start();

	std::string ServerIp = "127.0.0.1";
	int ServerPort = 8080;
	std::string LocalIp = "127.0.0.1";
	int LocalPort = 9000;
	ServiceMessage echoMessage;
	echoMessage.makeHeader("Echo", ServerIp, ServerPort, LocalIp, LocalPort, "echo", "textMsg");
	echoMessage.makeContent("This is a text message.");
	ServiceMessage fileMessage1;
	fileMessage1.makeHeader("Upload", ServerIp, ServerPort, LocalIp, LocalPort, "file", "zoobar1.txt");

	dis->PostMsg(echoMessage);
	dis->PostMsg(fileMessage1);
}
#endif