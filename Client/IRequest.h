/////////////////////////////////////////////////////////////////
// IRequest.h - interface for component using C++11 thread     //
//                                                             //
// Yufan Gong, CSE687 - Object Oriented Design, Spring 2014    //
/////////////////////////////////////////////////////////////////

#ifndef IREQUEST_H
#define IREQUEST_H

#include "../Message/Message.h"

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

struct DLL_DECL IRequest
{
	virtual ~IRequest() {}
	static IRequest* Create(std::string localip, int localport);
	virtual void PostServiceMessage(const ServiceMessage& msg) = 0;
	virtual ServiceMessage GetServiceMessage() = 0;
	virtual std::string GetMessageContent() = 0;
	virtual ServiceMessage MakeMessage(std::string Command, std::string path, std::string content) = 0;
	virtual ServiceMessage MakeMessage(std::string Command, std::string path, std::string content, int threadNum) = 0;
	virtual void processing(int argc, char* argv[]) = 0;
};
#endif