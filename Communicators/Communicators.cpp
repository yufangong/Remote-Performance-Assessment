/////////////////////////////////////////////////////////////////////////////
// Communicators.cpp - base classes for message-passing communicator       //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.1													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
///////////////////////////////////////////////////////////////////////////// 

#include "Communicators.h"

using Block = FileSystem::Block;

//----< get dispatcher pointer to put messages into dispatcher's blockingqueue>----------------

void EchoCommunicator::getDispatcher(AbstractCommunicators* dispatcher)
{
	dispatcher_ = dispatcher;
}

//----< get and process message from dispatcher>----------------

void EchoCommunicator::ProcessMsg()
{
	while (true)
	{
		ServiceMessage message = GetMsg();
		showMessageStatus(message);
		Timer timer;
		timer.start();
		if (message.GetCommand() == "Echo")
		{
			ServiceMessage reply;
			reply.makeHeader("reply", message.GetSourceIp(), message.GetSourcePort(), message.GetIp(), message.GetPort(), "client-echo", "textMsg");
			reply.makeContent("Server has received your request.");
			timer.stop();
			long time = timer.elapsedTime<std::chrono::milliseconds>();
			reply.SetTime(time);
			dispatcher_->PostMsg(reply);
			dp->ShowServerReply(0);
		}
		else if (message.GetCommand() == "Files")
		{
			std::string condition = message.GetPath();
			cl = hp.ParseCondition(condition);
			FileMger fm;
			std::vector<std::string> result = fm.FindFiles(cl);
			std::string results;
			for (auto r : result)
			{
				results += r;
				results += "|";
			}
			ServiceMessage reply;
			reply.makeHeader("reply_files", message.GetSourceIp(), message.GetSourcePort(), message.GetIp(), message.GetPort(), "client-echo", results);
			timer.stop();
			long time = timer.elapsedTime<std::chrono::milliseconds>();
			reply.SetTime(time);
			dispatcher_->PostMsg(reply);
			dp->ShowServerReply(1);
		}
		else if (message.GetCommand() == "Stop")
		{
			ServiceMessage reply;
			reply.makeHeader("Stop",message.GetSourceIp(), message.GetSourcePort(), message.GetIp(), message.GetPort(), "client-echo", "Stop");
			dispatcher_->PostMsg(reply);
		}
	}
}
void EchoCommunicator::showMessageStatus(ServiceMessage message)
{
	if (message.GetCommand() == "Echo")
		dp->ShowServerRecieve(0);
	else if (message.GetCommand() == "Files")
		dp->ShowServerRecieve(1);
	else if (message.GetCommand() == "Stop")
		dp->ShowServerRecieve(6);
}

//----< get dispatcher pointer to put messages into dispatcher's blockingqueue>----------------

void FileCommunicator::getDispatcher(AbstractCommunicators* dispatcher)
{
	dispatcher_ = dispatcher;
}

//----< get and process message from dispatcher>----------------

void FileCommunicator::ProcessMsg()
{
	while (true)
	{
		ServiceMessage message = GetMsg();

		if (message.GetCommand() == "Upload")
		{
			RecieveFile(message);
		}
		else if (message.GetCommand() == "Download")
		{
			dp->ShowServerRecieve(3);

			ServiceMessage msg;
			msg.makeHeader("reply_download", message.GetSourceIp(), message.GetSourcePort(), message.GetIp(), message.GetPort(), "client-file", message.GetPath());
			msg.makeContent(message.getStringContent());
			SendFile(msg);
		}
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

			ServiceMessage reply;
			reply.makeHeader("reply_upload", message.GetSourceIp(), message.GetSourcePort(), message.GetIp(), message.GetPort(), "client-echo", "textMsg");
			reply.makeContent("Server has received your file: " + message.GetPath());

			timer.stop();
			long time = timer.elapsedTime<std::chrono::milliseconds>();
			reply.SetTime(time);

			dispatcher_->PostMsg(reply);
			dp->ShowServerReply(2);
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
		File* file = new File("../Server/Repository/" + filename);
		fileBuffer[filename] = file;
		file->open(FileSystem::File::out, FileSystem::File::binary);
		FileSystem::Block block = message.GetContent();
		if (file->isGood())
		{
			file->putBlock(block);
			dp->ShowServerRecieve(2);
			timer.start();
		}
	}
}

//----< read file by blocks and warp them in messages >----------------

void FileCommunicator::SendFile(ServiceMessage message)
{
	Timer timer;
	timer.start();


	std::string filename = message.GetPath();
	std::string shortfilename = message.getStringContent();
	int blocksize = 1024;
	FileSystem::File file(filename);
	file.open(FileSystem::File::in, FileSystem::File::binary);

	if (!file.isGood())
	{
		std::cout << "\n  can't open executable\n";
		std::cout << "\n  looking for:\n  ";
		std::cout << FileSystem::Path::getFullFileSpec(file.name()) << "\n";
	}
	else
	{
		while (file.isGood())
		{
			FileSystem::Block blocks = file.getBlock(blocksize);
			message.SetPath(shortfilename);
			message.makeContent(blocks);
			dispatcher_->PostMsg(message);
		}

		timer.stop();
		long time = timer.elapsedTime<std::chrono::milliseconds>();
		message.SetTime(time);

		message.SetStatus("finish");
		message.SetPath(shortfilename);
		message.makeContent("Download process completed.");
		dispatcher_->PostMsg(message);

		dp->ShowServerReply(3);
	}
}

//----< get dispatcher pointer to put messages into dispatcher's blockingqueue>----------------

void SimilarityCommunicator::getDispatcher(AbstractCommunicators* dispatcher)
{
	dispatcher_ = dispatcher;
}

//----< get and process message from dispatcher>----------------

void SimilarityCommunicator::ProcessMsg()
{

	while (true)
	{
		std::string similarityResult;
		ServiceMessage message = GetMsg();
		dp->ShowServerRecieve(4);
		IExec* pIExec;
		pIExec = IExec::Create();
		Timer timer;
		timer.start();

		std::cout << "Server gets message.\n";
		std::string condition = message.GetPath();
		cl = hp.ParseCondition(condition);
		pIExec->Analysis(cl);
		similarityResult = pIExec->getResultToString();
		ServiceMessage reply;
		reply.makeHeader("reply_simi", message.GetSourceIp(), message.GetSourcePort(), message.GetIp(), message.GetPort(), "client-echo", "null");
		reply.makeContent(similarityResult);

		timer.stop();
		long time = timer.elapsedTime<std::chrono::milliseconds>();
		reply.SetTime(time);

		dispatcher_->PostMsg(reply);

		dp->ShowServerReply(4);
	}
}

//----< get dispatcher pointer to put messages into dispatcher's blockingqueue>----------------

void SearchCommunicator::getDispatcher(AbstractCommunicators* dispatcher)
{
	dispatcher_ = dispatcher;
}

//----< get and process message from dispatcher>----------------

void SearchCommunicator::ProcessMsg()
{
	while (true)
	{
		ServiceMessage message = GetMsg();
		dp->ShowServerRecieve(5);

		Timer timer;
		timer.start();

		std::string condition = message.GetPath();
		cl = hp.ParseCondition(condition);
		TextSearch ts;
		std::string result = ts.StartTextSearch(message.GetThreadNumber(), message.getStringContent(), cl);
		ServiceMessage reply;
		reply.makeHeader("reply_search", message.GetSourceIp(), message.GetSourcePort(), message.GetIp(), message.GetPort(), "client-echo", result);
		
		timer.stop();
		long time = timer.elapsedTime<std::chrono::milliseconds>();
		reply.SetTime(time);

		dispatcher_->PostMsg(reply);

		dp->ShowServerReply(5);
	}
}

//----< parse condition >----------------

CommandLine HelpClass::ParseCondition(std::string condition)
{
	Path path;
	std::string temp;
	Patterns patterns;
	Options options;
	size_t i, j;
	for (i = 0; i < condition.size(); i++)
	{
		if (condition[i] != '|' && condition[i] != ' ')
			path += condition[i];
		else
			break;
	}
	for (j = i + 1; j < condition.size(); j++)
	{
		if (condition[j] != '|' && condition[j] != ' ')
			temp += condition[j];
		else if (condition[j] == ' ')
		{
			patterns.push_back(temp);
			temp = "";
		}
		else if (condition[j] == '|')
		{
			patterns.push_back(temp);
			temp = "";
			break;
		}
	}
	options = parseCondition(j, condition);
	path = "../Server/Repository";
	return CommandLine{ path, patterns, options };
}

//----< parse condition >----------------

Options HelpClass::parseCondition(int j, std::string condition)
{
	std::string temp;
	Options options;
	for (size_t k = j + 1; k < condition.size(); k++)
	{
		if (condition[k] != '|' && condition[k] != ' ')
			temp += condition[k];
		else if (condition[k] == ' ')
		{
			options.push_back(temp);
			temp = "";
		}
		else if (condition[k] == '|')
		{
			options.push_back(temp);
			temp = "";
			break;
		}
	}
	return options;
}
#ifdef TEST_COMMUNICATORS

//----< test stub >--------------------------------------------

//#include "../Dispatcher/Dispatcher.h"
//test other functions in communicators-client.cpp, test dispatcher
//in dispatcher.cpp

int main()
{
	EchoCommunicator* echo = new EchoCommunicator();
	//CommunicatorDispatcher* dis = new CommunicatorDispatcher();
	//dis->Register(echo);
	ServiceMessage echoMessage;
	echoMessage.makeHeader("Echo", "ip", 0, "ip", 0, "echo", "textMsg");
	echoMessage.makeContent("This is a text message.");

	echo->PostMsg(echoMessage);

	FileCommunicator* file = new FileCommunicator();
	//dis->Register(file);
	Block content;

	ServiceMessage filemessage;
	filemessage.makeHeader("Download", "ip", 0, "ip", 0, "echo", "serverPic.jpg");
	filemessage.makeContent(content);
	file->PostMsg(filemessage);

	ServiceMessage filemessage2;
	filemessage2.makeHeader("Upload", "ip", 0, "ip", 0, "echo", "serverPic.jpg");
	filemessage2.makeContent(content);
	file->PostMsg(filemessage2);

	//echo->getDispatcher(dis);
	echo->Start();
	//file->getDispatcher(file);
	file->Start();

	while (true);
}
#endif