/////////////////////////////////////////////////////////////////////////////
// messages.cpp - construct http style messages to serve with message pass //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.1													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
///////////////////////////////////////////////////////////////////////////// 

#include "Message.h"
#include <iostream>

//----< construct message header >--------

void ServiceMessage::makeHeader(std::string command, std::string target_ip, int target_port, std::string source_ip, 
	int source_port, std::string target_communicator, std::string path)
{
	header._command = command;
	header._target_ip = target_ip;
	header._target_port = target_port;
	header._source_ip = source_ip;
	header._source_port = source_port;
	header._target_communicator = target_communicator;
	header._path = path;
}
//----< set target ip and port >--------

void ServiceMessage::SetTargetUrl(std::string targetIp, int targetport)
{
	header._target_ip = targetIp;
	header._target_port = targetport;
}

//----< set source ip and port >--------

void ServiceMessage::SetSourceUrl(std::string sourceIp, int sourcePort)
{
	header._source_ip = sourceIp;
	header._source_port = sourcePort;
}
//----< set filename or search and similarity analysis paths >--------

void ServiceMessage::SetPath(std::string filename)
{
	header._path = filename;
}

//----< set message path >--------

void ServiceMessage::SetPath(std::string path, std::string patterns, std::string options)
{
	header._path = path + "|" + patterns + "|" + options + "|";
}

//----< return message body (string) >--------

std::string ServiceMessage::getStringContent()
{
	std::string content;
	for (size_t i = 0; i < _content.size(); i++)
	{
		content.push_back(_content[i]);
	}
	return content;
}
//----< construct message body (file) >--------

void ServiceMessage::makeContent(std::string content)
{
	_content = 0;
	for (size_t i = 0; i < content.size(); i++)
	{
		_content.push_back(content[i]);
	}

	header._content_length = content.size();
}

//----< construct message body (text string) >--------

void ServiceMessage::makeContent(Block content)
{
	this->_content = content;
	this->header._content_length = content.size();
}

//----< convert message header to string >--------

std::string ServiceMessage::MsgHeaderToString()
{
	std::string msg_ = "";
	msg_.append(header._command);
	msg_.append("\n");
	msg_.append(header._target_ip);
	msg_.append("\n");
	msg_.append(IntToString(header._target_port));
	msg_.append("\n");
	msg_.append(header._source_ip);
	msg_.append("\n");
	msg_.append(IntToString(header._source_port));
	msg_.append("\n");
	msg_.append(header._target_communicator);
	msg_.append("\n");
	msg_.append(header._path);
	msg_.append("\n");
	msg_.append(IntToString(header._threadnumber));
	msg_.append("\n");
	msg_.append(header._status);
	msg_.append("\n");
	msg_.append(LongToString(header._processtime));
	msg_.append("\n");
	msg_.append(IntToString(header._content_length));
	msg_.append("\n");
	msg_.append("\n");
	return msg_;
}

//----< reconstruct message header line by line >--------

ServiceMessage ServiceMessage::rebuildMsgHeader(std::string str, int count)
{
	switch (count)
	{
	case 0:
		str.erase(str.size() - 1, str.size());
		this->header._command = str;
		break;
	case 1:
		str.erase(str.size() - 1, str.size());
		this->header._target_ip = str;
		break;
	case 2:
		this->header._target_port = atoi(str.c_str());
		break;
	case 3:
		str.erase(str.size() - 1, str.size());
		this->header._source_ip = str;
		break;
	case 4:
		this->header._source_port = atoi(str.c_str());
		break;
	default:
		*this = rebuildMsgHeader2(str, count);
		break;
	}
	return *this;
}

//----< just intend to not over cc >--------

ServiceMessage ServiceMessage::rebuildMsgHeader2(std::string str, int count)
{
	switch (count)
	{
	case 5:
		str.erase(str.size() - 1, str.size());
		this->header._target_communicator = str;
		break;
	case 6:
		str.erase(str.size() - 1, str.size());
		this->header._path = str;
		break;
	case 7:
		this->header._threadnumber = atoi(str.c_str());
		break;
	case 8:
		str.erase(str.size() - 1, str.size());
		this->header._status = str;
		break;
	case 9:
		this->header._processtime = atoi(str.c_str());
		break;
	case 10:
		this->header._content_length = atoi(str.c_str());
		break;
	default:
		break;
	}
	return *this;
}

//----< reconstruct message body (text string) >--------

ServiceMessage ServiceMessage::rebuildMsgBody(char body[])
{
	_content = 0;
	for (int i = 1; i < header._content_length + 1; i++)
	{
		_content.push_back(body[i]);
	}
	return *this;
}

//----< convert block to char* to send >--------

char* ServiceMessage::SendContent()
{ 
	char* body = new char[header._content_length+2];
	body[0] = '<';
	for (int i = 0; i < header._content_length; i++)
	{
		body[i+1] = _content[i];
	}
	body[header._content_length + 1] = '>';
	return body;
}

//----< convert int to string >--------

std::string ServiceMessage::IntToString(const int num)
{
	std::ostringstream out;
	out << num;
	return out.str();
}

//----< convert long to string >--------

std::string ServiceMessage::LongToString(const long num)
{
	std::ostringstream out;
	out << num;
	return out.str();
}

//----< help function: print header >--------

void ServiceMessage::PrintHeader()
{
	std::cout << this->header._command << std::endl << this->header._target_ip << std::endl << this->header._target_port
		<< std::endl << this->header._source_ip << std::endl << this->header._source_port << std::endl << this->header._target_communicator << std::endl
		<< this->header._path << std::endl << this->header._status << std::endl << this->header._content_length << std::endl;
}

#ifdef TEST_MESSAGE

//----< test stub >--------

#include "../Display/Display.h"

int main()
{
	ServiceMessage message1;
	message1.makeHeader("command", "ip", 0, "ip", 0, "echo", "text");
	message1.makeContent("this is content");
	message1.PrintHeader();
	std::string str = message1.IntToString(message1.GetLength());
	std::cout << str;

	Block content;
	ServiceMessage message2;
	message2.makeHeader("command", "ip", 0, "ip", 0, "echo", "text");
	message2.makeContent(content);
	char* block = new char[10];
	block = message2.SendContent();
	ServiceMessage m3;
	m3 = m3.rebuildMsgBody(block);
	message2.PrintHeader();
	Display dp;
	dp.ShowMessage(1, message1);
	dp.ShowMessage(1, message2);
	dp.ShowMessage(1, m3);
}
#endif // TEST_MESSAGE
