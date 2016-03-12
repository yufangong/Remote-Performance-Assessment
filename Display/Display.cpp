/////////////////////////////////////////////////////////////////////////////
// Display.cpp -  Display package used to print output to the screen.      //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.1													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#include "Display.h"

//----< help function show title >-------------------

void Display::Title(std::string option)
{
	std::cout << "\n " << std::string(60, '=');
	std::cout << "\n " << option << " Side Start";
	std::cout << "\n " << std::string(60, '=');

}

//----< help function show server recieve status >-------------------

void Display::ShowServerRecieve(int i)
{
	switch (i)
	{
	case 0:
		std::cout << "\n Server Echo communicator recieved client's Echo message. \n";
		break;
	case 1:
		std::cout << "\n Server Echo communicator recieved client's Get Files request. \n";
		break;
	case 2:
		std::cout << "\n Server File Communicator recieved client's Upload File request. \n";
		break;
	case 3:
		std::cout << "\n Server File Communicator recieved client's Download File request. \n";
		break;
	case 4:
		std::cout << "\n Server Similarity Communicator recieved client's Similarity Analysis request. \n";
		break;
	case 5:
		std::cout << "\n Server Search Communicator recieved client's Text Search request. \n";
		break;
	case 6:
		std::cout << "\n Server Search Communicator recieved client's Stop message. \n";
		break;
	default:
		break;
	}
}

//----< help function show server reply status >-------------------

void Display::ShowServerReply(int i)
{
	switch (i)
	{
	case 0:
		std::cout << "\n Server Echo communicator replied client's Echo message. \n";
		break;
	case 1:
		std::cout << "\n Server Echo communicator replied client's Get Files request. \n";
		break;
	case 2:
		std::cout << "\n Server File Communicator replied client's Upload File request. \n";
		break;
	case 3:
		std::cout << "\n Server File Communicator replied client's Download File request. \n";
		break;
	case 4:
		std::cout << "\n Server Similarity Communicator replied client's Similarity Analysis request. \n";
		break;
	case 5:
		std::cout << "\n Server Search Communicator replied client's Text Search request. \n";
		break;
	default:
		break;
	}
}

//----< help function show client request status >-------------------

void Display::ShowClientRequest(int i)
{
	switch (i)
	{
	case 0:
		std::cout << "\n Client sent an Echo text message. \n";
		break;
	case 1:
		std::cout << "\n Client sent a Get Files request. \n";
		break;
	case 2:
		std::cout << "\n Client sent an Upload or Download File request. \n";
		break;
	case 4:
		std::cout << "\n Client sent a Similarity Analysis request. \n";
		break;
	case 5:
		std::cout << "\n Client sent a Text Search request. \n";
		break;
	default:
		break;
	}
}

//----< help function show client recieve status >-------------------

void Display::ShowClientRecieve(int i)
{
	switch (i)
	{
	case 0:
		std::cout << "\n Client recieved an Echo text message. \n";
		break;
	case 1:
		std::cout << "\n Client recieved Files Names from server. \n";
		break;
	case 2:
		std::cout << "\n Client recieved Upload reply. \n";
		break;
	case 3:
		std::cout << "\n Client recieved Download File. \n";
		break;
	case 4:
		std::cout << "\n Client recieved Similarity result. \n";
		break;
	case 5:
		std::cout << "\n Client recieved Text Search result. \n";
		break;
	default:
		break;
	}
}

//----< help function show message >-------------------

void Display::ShowMessage(size_t type, ServiceMessage message)
{
	//mtx_.lock();
	std::cout << "\n " << std::string(50, '*');
	switch (type)
	{
	case 1:
		std::cout << "\n Send text message from Client to Server.";
		break;
	case 2:
		std::cout << "\n Upload text file from Client to Server.";
		break;
	case 3:
		std::cout << "\n Upload binary file from Client to Server.";
		break;
	case 4:
		std::cout << "\n Send download file request to Server.";
		break;
	case 5:
		std::cout << "\n Send reply message from Server to Client.";
		break;
	case 6:
		std::cout << ":\n Echo Recieved reply message from Server.";
		break;
	case 7:
		std::cout << "\n " << message.GetTargetComm() << " Recieved request message from Client.";
		break;
	default:
		break;
	}
	std::cout << "\n " << std::string(50, '*');
	PrintMessage(message);
	//mtx_.unlock();
}

//----< help function show message header >-------------------

void Display::PrintMessage(ServiceMessage message)
{
	std::cout << "\n Command: " << message.GetCommand() << "\n Target: IP " << message.GetIp() << " Port " << message.GetPort()
		<< "\n Source: IP " << message.GetSourceIp() << " Port " << message.GetSourcePort();
	if (message.GetPath() != "textMsg")
	{
		std::cout << "\n File name: " << message.GetPath() << std::endl;
	}
	//if (message.GetCommand() == "Echo" || message.GetCommand() == "reply")
		PrintMsgBody(message);
}

//----< help function show message content >-------------------

void Display::PrintMsgBody(ServiceMessage message)
{
	std::cout << "\n Content: ";
	for (auto i : message.GetContent().getblock())
	{
		std::cout << i;
	}
	std::cout << "\n";
}
#ifdef TEST_DISPLAY

//----< test stub >-------------------

int main()
{
	std::string ServerIp = "127.0.0.1";
	int ServerPort = 8080;
	std::string LocalIp = "127.0.0.1";
	int LocalPort = 9000;
	ServiceMessage echoMessage;
	echoMessage.makeHeader("Echo", ServerIp, ServerPort, LocalIp, LocalPort, "echo", "textMsg");
	echoMessage.makeContent("This is a text message.");
	ServiceMessage fileMessage1;
	fileMessage1.makeHeader("Upload", ServerIp, ServerPort, LocalIp, LocalPort, "file", "zoobar1.txt");
	Display dp;
	dp.ShowMessage(1, echoMessage);
	dp.ShowMessage(2, fileMessage1);
}
#endif