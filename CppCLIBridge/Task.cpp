///////////////////////////////////////////////////////////////
// Task.cpp - Implementation of long running Task            //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
// Changed by Yufan Gong for OOD project4, Apr 29, 2014      //
///////////////////////////////////////////////////////////////

#include "Task.h"
#include "ITask.h"

//invoke connect function to connect with server

void Task::connect(std::string ip, int port)
{
	pIreq = IRequest::Create(ip, port);
}

//transfer message from wpf to client inQ

void Task::transferMessage(std::string command, std::string path, std::string content)
{
	timer.start();
	ServiceMessage request;
	request = pIreq->MakeMessage(command, path, content);
	pIreq->PostServiceMessage(request);
	if (command == "Download")
	{
		if (static_cast<ITask^>(hBridge_) != nullptr)  // did Bridge register itself?
			hBridge_->doCallBack(convert(getdownloadstatus()));
	}
	else if (command != "Stop")
	{
		if (static_cast<ITask^>(hBridge_) != nullptr)  // did Bridge register itself?
			hBridge_->doCallBack(convert(getReply()));
	}
}

//transfer message from wpf to client inQ

void Task::transferMessage(std::string command, std::string path, std::string content, int threadNum)
{
	timer.start();
	ServiceMessage request;
	request = pIreq->MakeMessage(command, path, content, threadNum);
	pIreq->PostServiceMessage(request);
	if (static_cast<ITask^>(hBridge_) != nullptr)  // did Bridge register itself?
		hBridge_->doCallBack(convert(getReply()));
}

//get reply from client outQ

std::string Task::getReply()
{
	std::string reply = pIreq->GetMessageContent();
	timer.stop();
	long time = timer.elapsedTime<std::chrono::milliseconds>();
	reply = LongToString(time) + "@" + reply;
	return reply;
}

//get download reply from client outQ

std::string Task::getdownloadstatus()
{
	std::string reply;

	reply = pIreq->GetMessageContent();

	timer.stop();
	long time = timer.elapsedTime<std::chrono::milliseconds>();
	reply = LongToString(time) + "@" + reply;

	//reply = "Download process complete.";
	return reply;
}

//----< convert long to string >--------

std::string Task::LongToString(const long num)
{
	std::ostringstream out;
	out << num;
	return out.str();
}


int main()
{
	Task t;
	t.transferMessage("Echo", "textMessage", "this is a text message.");
}

