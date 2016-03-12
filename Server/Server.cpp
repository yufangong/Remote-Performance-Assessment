/////////////////////////////////////////////////////////////////////////////
// Server.cpp -  Start and console of this project                         //
//               Executive on client side                                  //
// ----------------------------------------------------------------------- //
// Version:      1.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
#include "../Reciever/Reciever.h"
#include "../Communicators/Communicators.h"
//----< start program >--------------------------------------------

int main()
{
	Display dp;
	dp.Title("Server");
	int ret = 0;
	try
	{
		Receiver* rcvr = new Receiver();
		CommunicatorDispatcher* dispatcher = new CommunicatorDispatcher();
		EchoCommunicator* echo = new EchoCommunicator();
		echo->SetName("echo");
		dispatcher->Register(echo);
		FileCommunicator* file = new FileCommunicator();
		file->SetName("file");
		dispatcher->Register(file);
		SimilarityCommunicator* simi = new SimilarityCommunicator();
		simi->SetName("simi");
		dispatcher->Register(simi);
		SearchCommunicator* search = new SearchCommunicator();
		search->SetName("search");
		dispatcher->Register(search);
		dispatcher->Start();

		echo->getDispatcher(dispatcher);
		echo->Start();
		file->getDispatcher(dispatcher);
		file->Start();
		simi->getDispatcher(dispatcher);
		simi->Start();
		search->getDispatcher(dispatcher);
		search->Start();
		rcvr->getDispatcher(dispatcher);
		rcvr->Start(8080);
		while (true);
	}
	catch (std::exception)
	{
		std::cout << "\n\n  Error!\n";
		ret = 1;
	}
	catch (...)
	{
		std::cout << "\n  something bad happened";
		ret = 1;
	}
	std::cout << "\n\n";
	return ret;
}