/////////////////////////////////////////////////////////////////////////////
// Sender.cpp - Demonstration of concurrent socket connectors              //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      3.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
#include "Sender.h"

//----< connect with sockets >----------------

void Sender::Connect(std::string ip, int port)
{
	std::cout << "\n\n Sender started.";

	if (!s_.connect(ip, port))
	{
		std::cout << "\n Couldn't connect to " << ip << ":" << port << "\n" ;
		return;
	}
	else
	{
		std::cout << "\n Connectted to " << ip << ":" << port << "\n";
	}
}

//----< send messages >----------------

void Sender::ProcessMsg()
{

	ServiceMessage message;
	do
	{
		message = GetMsg();

		std::string str;
		str = message.MsgHeaderToString();
		if (!s_.writeLine(str))
		{
			std::cout << "\n  bad status in sending thread";
		}
		if (!s_.sendAll(message.SendContent(), message.GetLength() + 2))
		{
			std::cout << "\n bad status in sending thread";
		}
	} while (message.GetCommand()!= "Stop");

	s_.disconnect();
}

//----< sendfiles, chunk files >----------------

void Sender::SendFile(ServiceMessage message)
{
	std::string localfilename = message.GetPath();
	std::string shortfilename = message.getStringContent();
	int blocksize = 1024;
	FileSystem::File file(localfilename);
	file.open(FileSystem::File::in, FileSystem::File::binary);

	if (!file.isGood())
	{
		std::cout << "\n  can't open executable\n";
		std::cout << "\n  looking for:\n  ";
		std::cout << FileSystem::Path::getFullFileSpec(file.name()) << "\n";
	}
	else
	{
		if (file.isGood())
		{
			FileSystem::Block blocks = file.getBlock(blocksize);
			message.SetPath(shortfilename);
			message.SetStatus("begin");
			message.makeContent(blocks);
			PostMsg(message);
		}
		while (file.isGood())
		{
			FileSystem::Block blocks = file.getBlock(blocksize);
			message.SetPath(shortfilename);
			message.makeContent(blocks);
			PostMsg(message);
		}
		message.SetStatus("finish");
		message.SetPath(shortfilename);
		message.makeContent("");
		PostMsg(message);

	}
}

#ifdef TEST_SENDER
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