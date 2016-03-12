/////////////////////////////////////////////////////////////////////////////////
// RequestProcessor.cpp - aim to generate a Application file invoked by tester //
//																			   //
// Yufan Gong, OOD project4, Apr 29, 2014                                      //
/////////////////////////////////////////////////////////////////////////////////

#include "../Client/IRequest.h"

int main(int argc, char* argv[])
{
	try
	{
		std::cout << "\n ===================================";
		std::cout << "\n Client side start";
		std::cout << "\n ===================================";
		std::string localip = argv[1];
		int localport = atoi(argv[2]);
		IRequest* pIreq = IRequest::Create(localip, localport);
		for (int i = 0; i < 5; i++)
		{
			pIreq->processing(argc, argv);
		}
		ServiceMessage message;
		message = pIreq->MakeMessage("Stop", "null", "null");
		pIreq->PostServiceMessage(message);
		pIreq->GetMessageContent();
	}
	catch (std::exception)
	{
		std::cout << "\n Error!";
	}
}
