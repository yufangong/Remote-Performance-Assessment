/////////////////////////////////////////////////////////////////////////////
// Communicators - client.cpp - Abstract base for message-passing          //
//                              communicator                               //
// ----------------------------------------------------------------------- //
// Version:      1.1													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
///////////////////////////////////////////////////////////////////////////// 

#include "Communicators - client.h"


//----< get and process message from server>----------------

void EchoCommunicator::ProcessMsg()
{
	while (true)
	{
		ServiceMessage message = GetMsg();
		std::cout << " Client recieved server reply. \n";
		outputQ_.enQ(message);
	}
}

//----< get and process message from server>----------------

void FileCommunicator::ProcessMsg()
{
	while (true)
	{
		ServiceMessage message = GetMsg();
		if (message.GetCommand() == "reply_download")
			RecieveFile(message);
	}
}

//----< recieve file blocks and write them into local file >----------------

void FileCommunicator::RecieveFile(ServiceMessage message)
{
	fileIter = fileBuffer.find(message.GetPath());
	if (fileIter != fileBuffer.end())
	{
		if (message.GetStatus() == "finish")
		{
			fileIter->second->close();
			outputQ_.enQ(message);
			std::cout << "\n File: " << message.GetPath() << " has been stored in local. \n";
		}

		FileSystem::Block block = message.GetContent();
		if (fileIter->second->isGood() && message.GetStatus() != "finish")
		{
			fileIter->second->putBlock(block);
		}

	}
	else
	{
		std::string filename = message.GetPath();
		File* file = new File("../Client/Download/" + filename);
		fileBuffer[filename] = file;
		file->open(FileSystem::File::out, FileSystem::File::binary);
		FileSystem::Block block = message.GetContent();
		if (file->isGood())
		{
			file->putBlock(block);
		}
	}
}

#ifdef TEST_COMMUNICATORS

//----< test stub >--------------------------------------------

int main()
{
	EchoCommunicator* echo = new EchoCommunicator();

	ServiceMessage echoMessage;
	echoMessage.makeHeader("Echo", "ip", 0, "ip", 0, "echo", "textMsg");
	echoMessage.makeContent("This is a text message.");

	echo->PostMsg(echoMessage);
	echo->Start();

	FileCommunicator* file = new FileCommunicator();
	Block content;

	ServiceMessage filemessage;
	filemessage.makeHeader("reply_download", "ip", 0, "ip", 0, "echo", "123.txt");
	filemessage.makeContent(content);
	file->PostMsg(filemessage);
	file->Start();
	while (true);
}
#endif