///////////////////////////////////////////////////////////////
// Bridge.cpp - Supports Interop between C# and native C++   //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
// Changed by Yufan Gong for OOD project4, Apr 29, 2014      //
///////////////////////////////////////////////////////////////

#include "ITask.h"
#include "Task.h"

ref class Bridge : public ITask
{
public:

	//invoke connect function to connect with server

	Bridge(String^ ip, int port) : pTask(new Task(this)) 
	{
		pTask->connect(convert(ip), port);
	}
	~Bridge() { delete pTask; }

	//transfer message from wpf to bridge to client inQ
	virtual void run(System::Windows::Threading::Dispatcher^ disp, String^ Command, String^ Path, String^ Content) override
	{
		//::MessageBox(0, L"in Bridge::run", L"debug", MB_OK);
		dispatcher_ = disp;
		pTask->transferMessage(convert(Command), convert(Path), convert(Content));
	}

	//transfer message from wpf to bridge to client inQ

	virtual void runSearch(System::Windows::Threading::Dispatcher^ disp, String^ Command, String^ Path, String^ Content, int ThreadNumber) override
	{
		dispatcher_ = disp;
		pTask->transferMessage(convert(Command), convert(Path), convert(Content), ThreadNumber);

	}

	//call back reply

	virtual void doCallBack(String^ reply) override
	{
		//::MessageBox(0, L"in Bridge::doCallBack", L"debug", MB_OK);
		if (callback != nullptr)
		{
			dispatcher_->Invoke(callback, reply);
		}
	}
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
	Task* pTask;
	System::Windows::Threading::Dispatcher^ dispatcher_;
	
};

ITask^ ITask::Create(String^ ip, int port)
{
	return gcnew Bridge(ip, port);
}