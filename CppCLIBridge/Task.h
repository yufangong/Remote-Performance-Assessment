#ifndef TASK_H
#define TASK_H
///////////////////////////////////////////////////////////////
// Task.h - Public interface of long running Task            //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
// Changed by Yufan Gong for OOD project4, Apr 29, 2014      //
///////////////////////////////////////////////////////////////

#include "ITask.h"
#include <vcclr.h>
#include "../Client/IRequest.h"
#include "../Survival/ChronoTimer.h"
class Task
{
public:
	// constructor accepts reference to Bridge as an ITask

	Task(ITask^ hBridge = nullptr) : hBridge_(hBridge) {}

	void connect(std::string ip, int port);
	void transferMessage(std::string command, std::string path, std::string content);
	void transferMessage(std::string command, std::string path, std::string content, int threadNum);
	std::string getReply();
	std::string getdownloadstatus();
private:
	String^ convert(const std::string& s)
	{
		StringBuilder^ sb = gcnew StringBuilder();
		for (char ch : s)
			sb->Append(static_cast<wchar_t>(ch));
		return sb->ToString();
	}

	std::string convert(String^ s)
	{
		std::string temp;
		for (int i = 0; i < s->Length; ++i)
			temp += static_cast<char>(s[i]);
		return temp;
	}
	std::string LongToString(const long num);
	gcroot<ITask^> hBridge_; // reference to C++\CLI Bridge
	IRequest* pIreq;
	Timer timer;
};

#endif
