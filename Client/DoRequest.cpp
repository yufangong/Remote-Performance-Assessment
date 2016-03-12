/////////////////////////////////////////////////////////////////////////////
// DoReuquest.cpp -  Emit programmed requests to Server                    //
// ----------------------------------------------------------------------- //
// Version:      1.2													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "IRequest.h"
//#include <functional>
#include "../Reciever/Reciever.h"
#include "../Communicators/Communicators - client.h"
#include "../Display/Display.h"

/////////////////////////////////////////////////////////////////////////
// DoRequest class

class DoRequest : public IRequest
{
public:
	DoRequest(std::string localip, int localport);
	virtual ~DoRequest() { }
	void Start();
	virtual void PostServiceMessage(const ServiceMessage& msg);
	virtual ServiceMessage GetServiceMessage();
	virtual std::string GetMessageContent();
	virtual ServiceMessage MakeMessage(std::string Command, std::string path, std::string content);
	virtual ServiceMessage MakeMessage(std::string Command, std::string path, std::string content, int threadNum);
	bool OutQueueEmpty();
	void doRequest(const ServiceMessage& msg, size_t count);
	void doRequest(int argc, char* argv[]);
	void processing(int argc, char* argv[]);
private:
	void StartCommunicators();
	BlockingQueue<ServiceMessage> inQ;
	BlockingQueue<ServiceMessage> outQ;
	std::thread rpThread;
	Sender* _sndr;
	CommunicatorDispatcher* _dispatcher;
	Receiver _rcvr;
	EchoCommunicator* _echo;
	FileCommunicator* _file;
	std::string _ServerIp;
	int _ServerPort;
	std::string _LocalIp;
	int _LocalPort;
};
//----< factory for RequestProcessors >--------------------------------

IRequest* IRequest::Create(std::string localip, int localport) { return new DoRequest(localip, localport); }

//----< Constructor creates channel thread >----------------------


DoRequest::DoRequest(std::string localip, int localport) : _ServerIp("127.0.0.1"), _ServerPort(8080), _LocalIp(localip), _LocalPort(localport)
{
	StartCommunicators();
	std::function<void()> f = [&]()
	{
		ServiceMessage msg;
		while (true)
		{
			msg = inQ.deQ();
			if (msg.GetCommand() == "Upload")
			{
				_sndr->SendFile(msg);
			}
			else
			{
				_sndr->PostMsg(msg);
			}
		}
	};
	rpThread = std::thread(f);
	rpThread.detach();

}
//----< make message use parameters passed by wpf gui (except text search) >----------------------

ServiceMessage DoRequest::MakeMessage(std::string Command, std::string path, std::string content)
{
	ServiceMessage message;
	if (Command == "Echo" || Command == "Files")
	{
		message.makeHeader(Command, _ServerIp, _ServerPort, _LocalIp, _LocalPort, "echo", path);
		message.makeContent(content);
	}
	if (Command == "Upload" || Command == "Download")
	{
		message.makeHeader(Command, _ServerIp, _ServerPort, _LocalIp, _LocalPort, "file", path);
		message.makeContent(content);
	}
	if (Command == "Similarity")
	{
		message.makeHeader(Command, _ServerIp, _ServerPort, _LocalIp, _LocalPort, "simi", path);
	}
	if (Command == "Stop")
	{
		message.makeHeader(Command, _ServerIp, _ServerPort, _LocalIp, _LocalPort, "echo", path);
	}

	return message;
}

//----< make message use parameters passed by wpf gui (only text search) >----------------------

ServiceMessage DoRequest::MakeMessage(std::string Command, std::string path, std::string content, int threadNum)
{
	ServiceMessage message;
	if (Command == "TextSearch")
	{
		message.makeHeader(Command, _ServerIp, _ServerPort, _LocalIp, _LocalPort, "search", path); 
			message.SetThreadNumber(threadNum);
		message.makeContent(content);
	}
	return message;
}

//----< start all communicators >----------------------

void DoRequest::StartCommunicators()
{
	_dispatcher = new CommunicatorDispatcher();
	_sndr = new Sender();
	_echo = new EchoCommunicator(outQ);
	_file = new FileCommunicator(outQ);
	_dispatcher->SetName("dispatcher");
	_sndr->SetName("sender");
	_sndr->Connect(_ServerIp, _ServerPort);

	_sndr->Start();
	_echo->SetName("client-echo");
	_dispatcher->Register(_echo);

	_file->SetName("client-file");
	_dispatcher->Register(_file);

	_echo->Start();
	_file->Start();

	_dispatcher->Start();

	_rcvr.getDispatcher(_dispatcher);
	_rcvr.Start(_LocalPort);
}


//----< make multiple requests for specified message >-----------------

void DoRequest::doRequest(const ServiceMessage& msg, size_t count)
{
  for (size_t i = 0; i < count; ++i)
    PostServiceMessage(msg);
}

//----< parse commandline to make message and request >----------------

void DoRequest::doRequest(int argc, char* argv[])
{
	ServiceMessage message;
	Display dp;
	if (argc < 4)
		throw std::exception("pass command type and parameters on commandline");
	message.SetCommand(argv[3]);
	message.SetTargetUrl(_ServerIp, _ServerPort);
	message.SetSourceUrl(_LocalIp, _LocalPort);
	if (std::string(argv[3]) == "Echo")
	{
		message.SetPath("textMessage");
		message.makeContent(argv[4]);
		message.SetTargetComm("echo");
		dp.ShowClientRequest(0);
	}
	if (std::string(argv[3]) == "Files")
	{
		message.SetPath(argv[4], argv[5], argv[6]);
		message.SetTargetComm("echo");
		dp.ShowClientRequest(1);
	}
	else if (std::string(argv[3]) == "Upload" || std::string(argv[3]) == "Download")
	{
		message.SetPath(argv[4]);
		message.SetTargetComm("file");
		message.makeContent(argv[5]);
		dp.ShowClientRequest(2);
	}
	else if (std::string(argv[3]) == "Similarity")
	{
		message.SetPath(argv[4], argv[5], argv[6]);
		message.SetTargetComm("simi");
		dp.ShowClientRequest(4);
	}
	else if (std::string(argv[3]) == "TextSearch")
	{
		message.SetPath(argv[4], argv[5], argv[6]);
		message.SetThreadNumber(atoi(argv[7]));
		message.SetTargetComm("search");
		message.makeContent(argv[8]);
		dp.ShowClientRequest(5);
	}
	PostServiceMessage(message);
}

//----< post message to channel >---------------------------------

void DoRequest::PostServiceMessage(const ServiceMessage& msg)
{
  inQ.enQ(msg);
}
//----< get message from channel >--------------------------------

ServiceMessage DoRequest::GetServiceMessage()
{
  return outQ.deQ();
}

//----< get message content from channel and wpf to display>--------------------------------

std::string DoRequest::GetMessageContent()
{
	ServiceMessage message = outQ.deQ();
	std::string content;
	if (message.GetCommand() == "reply_files" || message.GetCommand() == "reply_search")
	{
		content = message.GetPath();
	}
	if (message.GetCommand() == "reply_simi" || message.GetCommand() == "reply_upload")
	{
		content = message.getStringContent();
	}
	if (message.GetCommand() == "reply_download")
	{
		content = message.GetStatus(); 
		while (content != "finish")
		{
			message = outQ.deQ();
		}
		content = message.getStringContent();
	}

	std::string time = message.LongToString(message.GetTime());
	std::string TimeAndContent = time + "@" + content;

	return TimeAndContent;
}

//----< is the output queue empty so I can shut down? >----------------

bool DoRequest::OutQueueEmpty()
{
  bool test = outQ.size() == 0;
  return test;
}
//----< demo the RequestProcessor >------------------------------------

void DoRequest::processing(int argc, char* argv[])
{
	try
	{
		doRequest(argc, argv);
		GetMessageContent();
	}
	catch (std::exception)
	{
		std::cout << "\n Invalid argument, check again.\n";
	}
}

//----< demonstrate RequestProcessor >---------------------------------
int main(int argc, char* argv[])
{
	int LocalPort = atoi(argv[2]);
	std::string LocalIp = argv[1];
	DoRequest rp(LocalIp, LocalPort);
	rp.processing(argc, argv);
	//ServiceMessage message = rp.MakeMessage("Stop", "path", "content");
	//rp.PostServiceMessage(message);
	//while (1);
}

