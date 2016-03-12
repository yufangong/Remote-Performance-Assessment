#ifndef MESSAGE_H
#define MESSAGE_H
/////////////////////////////////////////////////////////////////////////////
// messages.h - construct http style messages to serve with message pass   //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
///////////////////////////////////////////////////////////////////////////// 
/*
*  Package Operations:
*  -------------------
*  This package contains one class:
*  ServiceMessage
*    construct http style message to serve with message passing
*
*  Required Files:
*  - AbstractCommunicator.h   Defines Communicator behavior
*  - BlockingQueue.h          Defines queue process
*
*  Public Interface:
*  =================
*  makeHeader: create message header
*  makeContent: create message body
*  rebuildfunctions: rebuild message body and header
*  set, get functions
*
*  Build Command:   cl /EHa /DTEST_MESSAGE message.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : Apr 12, 2014
*  - first release
*/
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "../FileSystem/FileSystem.h"

using Block = FileSystem::Block;

///////////////////////////////////////////////////////////////////////
// ServiceMessage

class ServiceMessage
{
public:
	ServiceMessage(){ SetStatus("normal"); SetThreadNumber(1); SetTime(0); }
	void SetCommand(std::string command){ header._command = command; }
	std::string GetCommand(){ return header._command; }
	void SetStatus(std::string str){ header._status = str; }
	std::string GetStatus(){ return header._status; }
	void SetThreadNumber(int threadNum){ header._threadnumber = threadNum; }
	int GetThreadNumber(){ return header._threadnumber; }
	void SetPath(std::string filename);
	void SetPath(std::string path, std::string patterns, std::string options);
	std::string GetPath(){ return this->header._path; }
	std::string GetSourceIp(){ return this->header._source_ip; }
	int GetSourcePort(){ return this->header._source_port; }
	void SetTargetUrl(std::string targetIp, int localport);
	void SetSourceUrl(std::string localIp, int localport);
	std::string GetIp(){ return this->header._target_ip; }
	int GetPort(){ return this->header._target_port; }
	void SetTargetComm(std::string communicator){ header._target_communicator = communicator; }
	std::string GetTargetComm(){ return this->header._target_communicator; }
	int GetLength(){ return this->header._content_length; }
	void SetTime(long time){ this->header._processtime = time; }
	long GetTime(){ return this->header._processtime; }
	char* SendContent();
	Block GetContent(){ return _content; }
	std::string getStringContent();
	void makeHeader(std::string command, std::string target_ip, int target_port, std::string source_ip, int source_port, std::string target_communicator, std::string path);
	void makeContent(std::string content);
	void makeContent(Block content);
	std::string MsgHeaderToString();
	ServiceMessage rebuildMsgHeader(std::string str, int count);
	ServiceMessage rebuildMsgHeader2(std::string str, int count);
	ServiceMessage rebuildMsgBody(char body[]);
	std::string IntToString(const int num);
	std::string LongToString(const long num);
	void PrintHeader();
private:
	Block _content;
	struct Header
	{
		std::string _command;
		std::string _target_ip;
		int _target_port;
		std::string _source_ip;
		int _source_port;
		std::string _target_communicator;
		std::string _path;
		int _threadnumber;
		std::string _status;
		long _processtime;
		int _content_length;
	}header;
};
#endif
