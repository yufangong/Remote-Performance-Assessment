/////////////////////////////////////////////////////////////////////
// Sockets.cpp - Provides basic network communication services     //
// ver 3.2                                                         //
// Language:      Visual C++, 2005                                 //
// Platform:      Dell Dimension 9150, Windows XP Pro, SP 2.0      //
// Application:   Utility for CSE687 and CSE775 projects           //
// Author:        Jim Fawcett, Syracuse University, CST 2-187      //
//                (315) 443-3948, jfawcett@twcny.rr.com            //
/////////////////////////////////////////////////////////////////////

#include "Sockets.h"
#include <iostream>
#include <sstream>

#ifdef TRACING
  #include "..\threads\locks.h"
  #define TRACE(msg) sout << "\n  " << msg;
#else
  #define TRACE(msg) ;
#endif

long SocketSystem::count = 0;

//----< convert integer to string >----------------------------------

std::string IntToString(const int num)
{
  std::ostringstream out;
  out << num;
  return out.str();
}
//----< get socket error message string >----------------------------

std::string SocketSystem::GetLastMsg(bool WantSocketMsg) {

// ask system what type of error occurred

  DWORD errorCode;
  if(WantSocketMsg)
    errorCode = WSAGetLastError();
  else
    errorCode = GetLastError();
  if(errorCode == 0)
    return "no error";

// map errorCode into a system defined error string
    
  DWORD dwFlags = 
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER;
  LPCVOID lpSource = NULL;
  DWORD dwMessageID = errorCode;
  DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
  LPSTR lpBuffer;
  DWORD nSize = 0;
  va_list *Arguments = NULL;

  FormatMessage(
    dwFlags,lpSource,dwMessageID,dwLanguageId, 
    (LPTSTR)&lpBuffer,nSize,Arguments
  );

  std::string _msg(lpBuffer);
  LocalFree(lpBuffer);
  return _msg;
}
//
//----< load WinSock Library >---------------------------------------

SocketSystem::SocketSystem()
{
  if(count == 0)
  {
    TRACE("loading wsock32 library");
    WORD wVersionRequested = MAKEWORD(1,1); // requesting version 1.1
    WSAData wData;                          // startup data filled by WSAStartup
    int err = WSAStartup(wVersionRequested, &wData);
    if(err == SOCKET_ERROR)
      throw std::exception("initialization error: ");
  }
  InterlockedIncrement(&count);
}
//----< destructor unloads socket library >--------------------------

SocketSystem::~SocketSystem()
{ 
  try {
    if(InterlockedDecrement(&count) == 0)
    {
      TRACE("unloading wsock32 library");
      WSACleanup();
    }
  }
  catch(...) { /* don't allow exception to propagate on shutdown */}
}
//----< get host name >----------------------------------------------

std::string SocketSystem::getHostName()
{
  char buffer[256];
  gethostname(buffer,256);
  return buffer;
}
//
//----< get ip address of network machine >--------------------------

std::string SocketSystem::getIpFromName(const std::string& name)
{
  hostent* remoteHost = 0;
  SOCKADDR_IN tcpAddr;
  tcpAddr.sin_family = AF_INET;
  tcpAddr.sin_addr.s_addr = inet_addr(name.c_str());
  if(tcpAddr.sin_addr.s_addr == INADDR_NONE)
  {
    // name is not an ip address, so try to map name to address
    // via DNS

    hostent* remoteHost = gethostbyname(name.c_str());
    if(remoteHost == NULL)
      throw std::exception("invalid name");
    CopyMemory(
      &tcpAddr.sin_addr, 
      remoteHost->h_addr_list[0], 
      remoteHost->h_length
    );
  }
  return inet_ntoa(tcpAddr.sin_addr);
}
//----< get network name of machine from ip address >----------------

std::string SocketSystem::getNameFromIp(const std::string& ip)
{
  struct hostent* host = (hostent*)malloc(sizeof(struct hostent));
  struct in_addr *ipaddr = (in_addr*)malloc(sizeof(struct in_addr));
  ipaddr->s_addr = inet_addr(ip.c_str());
  host = gethostbyaddr((char*)ipaddr, sizeof(ipaddr), AF_INET);
  if(!host)
    throw std::exception("name resolution error: ");    
  return host->h_name;
}
//----< constructor creates TCP Stream socket >----------------------

Socket::Socket()
{
  s_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(s_ == INVALID_SOCKET)
    throw std::exception("invalid socket in constructor");
}
//----< copy constructor >-------------------------------------------

Socket::Socket(const Socket& sock)
{
  TRACE("copying socket");
  DuplicateHandle(GetCurrentProcess(),(HANDLE)sock.s_,GetCurrentProcess(),(HANDLE*)&s_,0,false,DUPLICATE_SAME_ACCESS);
  //std::cout << "\n  source handle = " << sock.s_;
  //std::cout << "\n  destin handle = " << s_;
}
//----< promotes WinSock SOCKET handle to Socket object >------------

Socket::Socket(SOCKET s) : s_(s) {}

//----< destructor closes socket handle >----------------------------

Socket::~Socket()
{ 
  TRACE("destroying socket");
  closesocket(s_);
  //disconnect();
}
//----< assignment >-------------------------------------------------

Socket& Socket::operator=(const Socket& sock)
{
  if(this == &sock) return *this;
  TRACE("copying socket");
  DuplicateHandle(GetCurrentProcess(),(HANDLE)sock.s_,GetCurrentProcess(),(HANDLE*)&s_,0,false,DUPLICATE_SAME_ACCESS);
  return *this;
}
//----< assignment >-------------------------------------------------

Socket& Socket::operator =(SOCKET sock)
{
  TRACE("assigning from SOCKET");
  s_ = sock;
  return *this;
}
//----< connects to IP address or network host >---------------------

bool Socket::connect(std::string url, int port, bool throwError, size_t MaxTries)
{
  try {
    if(isalpha(url[0]))
      url = SocketSystem().getIpFromName(url);
  }
  catch(...)
  {
    if(throwError)
      throw std::exception(ss_.GetLastMsg(true).c_str());
    return false;
  }
  SOCKADDR_IN tcpAddr;
  tcpAddr.sin_family = AF_INET;
  tcpAddr.sin_addr.s_addr = inet_addr(url.c_str());
  tcpAddr.sin_port = htons(port);
  if(s_ == INVALID_SOCKET)
  {
    s_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  }
  size_t tryCount = 0;
  std::string countStr;
  while(true)
  {
    ++tryCount;
    TRACE("attempt to connect #" + IntToString(tryCount));
    int err = ::connect(s_, (sockaddr*)&tcpAddr, sizeof(tcpAddr));
     //std::string rip = System().getRemoteIP(this);
    int rport = System().getRemotePort(this);

    // error return from connect does not appear to be reliable
  
    if(err != SOCKET_ERROR && rport != -1)
      break;
    if(tryCount >= MaxTries)
    {
      if(throwError)
        throw std::exception(ss_.GetLastMsg(true).c_str());
      return false;
    }
    ::Sleep(100);
  }
  return true;
}
//
//----< disconnect socket >------------------------------------------

void Socket::disconnect()
{
  shutdown(s_, SD_BOTH); 
  closesocket(s_);
  s_ = INVALID_SOCKET;
}
//----< casts Socket to WinSock SOCKET handle >----------------------

Socket::operator SOCKET ()
{ 
  return s_; 
}
//----< send byte block >--------------------------------------------

int Socket::send(const char* block, size_t len)
{
  return ::send(s_,block,len,0);
}
//----< recieve byte block >-----------------------------------------

int Socket::recv(char* block, size_t len)
{
  return ::recv(s_,block,len,0);
}
//----< return number of bytes waiting >-----------------------------

int Socket::bytesLeft()
{
  unsigned long bytes;
  ::ioctlsocket(s_,FIONREAD,&bytes);
  return bytes;
}
//----< send blocks until all characters are sent >------------------

bool Socket::sendAll(const char* block, size_t len, bool throwError)
{
  size_t bytesSent;       // current number of bytes sent
  size_t blockIndx = 0;   // place in buffer to send next
  size_t count = 0;       // number of send failures

  const int sendRetries = 100;
  size_t blockLen = len;
  size_t bytesLeft = blockLen;
  while(bytesLeft > 0) {
    bytesSent = ::send(s_,&block[blockIndx],static_cast<int>(bytesLeft),0);
	if (bytesSent == SOCKET_ERROR)
	{
		mtx_.lock();
		std::cout << "\n  socket error";
		mtx_.unlock();
		++count;
		Sleep(50);
		bytesSent = 0;
	}
    //sout << "\n  sending retry";
	if (bytesSent == WSAECONNRESET)
	{
		mtx_.lock();
		std::cout << "\n  connection broken";
		mtx_.unlock();
		if (throwError)
			throw std::exception("connection closed");
		return false;
	}
    if(count==sendRetries)
    {
		mtx_.lock();
		std::cout << "\n  reached max retries";
	  mtx_.unlock();

      if(throwError)
        throw std::exception("send failed after 100 retries");
      return false;
    }
    //sout << "\n  sending succeeded";
    bytesLeft -= bytesSent;
    blockIndx += bytesSent;
  }
  return true;
}
//----< blocks until len characters have been sent >-----------------

bool Socket::recvAll(char* block, size_t len, bool throwError)
{
  const size_t recvRetries = 100;
  size_t bytesRecvd, bytesLeft = len;
  size_t blockIndx = 0, count = 0;
  while(bytesLeft > 0) {
    bytesRecvd = ::recv(s_,&block[blockIndx],static_cast<int>(bytesLeft),0);
    if(bytesRecvd == 0)
    {
      if(throwError)
        throw(std::exception("remote connection closed"));
      return false;
    }
    if(bytesRecvd == SOCKET_ERROR) {
      ++count;
      Sleep(50);
    }
    if(bytesRecvd == WSAECONNRESET)
    {
      if(throwError)
        throw(std::exception("connection closed"));
      return false;
    }
    if(count==recvRetries)
    {
      if(throwError)
        throw(std::exception("recv failed after 100 retries"));
      return false;
    }
    bytesLeft -= bytesRecvd;
    blockIndx += bytesRecvd;
  }
  return true;
}
//----< write a line of text >---------------------------------------
/*
 * appends newline if none at end of string
 *
 */
//add <> to string?
//bool Socket::writeLine(const std::string& str)
//{
//	std::string temp = "<begin>";
//	temp.append(str);
//	temp.append("<end>");
//	if (!sendAll(temp.c_str(), temp.size()))
//		return false;
//	return true;
//}
bool Socket::writeLine(const std::string& str)
{
  char end = str[str.size()-1];
  if(end != '\n' && end != '\r')
  {
    std::string temp = str;
    temp.append("\n");
    if(!sendAll(temp.c_str(), temp.size()))
      return false;
  }
  else
  {
    if(!sendAll(str.c_str(), str.size()))
      return false;
  }
  return true;
}
//----< read a line of text >----------------------------------------
/*
 * - removes ending newline if present, else reads to end of buffer
 * - returns empty string if not successful
 *
 */
std::string Socket::readLine()
{
	std::string temp;
	char block[1];
	//while(bytesLeft() > 0)  // don't block
	while (true)
	{
		if (!recvAll(block, 1))
			return "";
		// save all chars that are not newlines or carriage returns
		if (block[0] != '\n' && block[0] != '\r')
			temp += block[0];
		else
		{
			temp += '\n';
			if (bytesLeft() > 0)
			{
				char last = block[0];
				// remove remaining newline or carriage return if next in buffer
				::recv(s_, block, 1, MSG_PEEK);
				if (last == '\n' && block[0] == '\r')
					recv(block, 1);
				else if (last == '\r' && block[0] == '\n')
					recv(block, 1);
			}
			return temp;
		}
	}
	return temp;
}

//----< get local ip address >---------------------------------------

std::string SocketSystem::getLocalIP()
{
  //struct sockaddr name;
  //int len = sizeof(name);
  hostent* local = gethostbyname("");
  return inet_ntoa(*(struct in_addr*)*local->h_addr_list);
}
//----< get local port >---------------------------------------------

int SocketSystem::getLocalPort(Socket* pSock)
{
  struct sockaddr name;
  int len = sizeof(name);
  int status = getsockname(*pSock,&name,&len);
  if(status == 0)
  {
    struct sockaddr_in* pLocal = reinterpret_cast<sockaddr_in*>(&name);
    return htons(pLocal->sin_port);
  }
  return -1;
}
//----< get remote ip address >--------------------------------------

std::string SocketSystem::getRemoteIP(Socket* pSock)
{
  struct sockaddr name;
  int len = sizeof(name);
  int status = getpeername(*pSock,&name,&len);
  if(status == 0)
  {
    struct sockaddr_in* pRemote = reinterpret_cast<sockaddr_in*>(&name);
    return inet_ntoa(pRemote->sin_addr);
  }
  return "";
}
//----< get remote port >--------------------------------------------

int SocketSystem::getRemotePort(Socket* pSock)
{
  struct sockaddr name;
  int len = sizeof(name);
  int status = getpeername(*pSock,&name,&len);
  if(status == 0)
  {
    struct sockaddr_in* pRemote = reinterpret_cast<sockaddr_in*>(&name);
    return htons(pRemote->sin_port);
  }
  return -1;
}
//
//----< starts listener socket listening for connections >-----------

SocketListener::SocketListener(int port) : InvalidSocketCount(0)
{
  tcpAddr.sin_family = AF_INET;   // TCP/IP
  tcpAddr.sin_port = htons(port); // listening port
  tcpAddr.sin_addr.s_addr = INADDR_ANY;
                                  // listen over every network interface
  int err = bind(s_, (SOCKADDR*)&tcpAddr, sizeof(tcpAddr));

  if(err == SOCKET_ERROR)
  {
    throw std::exception("binding error type:");
  }

  /////////////////////////////////////////////////////////////////
  // listen for incoming connection requests

  int backLog = 10;
  err = listen(s_, backLog);

  if(err == SOCKET_ERROR)
    throw std::exception("listen mode error");
}
//----< destructor closes socket >-----------------------------------

SocketListener::~SocketListener()
{
  try {
    TRACE("destroying SocketListener");
    shutdown(s_, SD_BOTH);
    closesocket(s_);
  }
  catch(...) { /* don't let exception propagate on shutdown */}
}
//----< blocks until a connection request has been received >--------

SOCKET SocketListener::waitForConnect()
{
  const long MaxCount = 20;
  InvalidSocketCount = 0;
  TRACE("listener waiting for connection request");
  int size = sizeof(tcpAddr);
  SOCKET toClient;
  do {
    toClient = accept(s_, (SOCKADDR*)&tcpAddr, &size); 
    ++InvalidSocketCount;
    if(InvalidSocketCount >= 20)
      throw std::exception("invalid socket connection");
  } while (toClient == INVALID_SOCKET);
  TRACE("connection establishted");
  return toClient;
}
//
//----< shuts down listerner >---------------------------------------

void SocketListener::stop()
{
  TRACE("shutting down listener in SocketListerer");
  shutdown(s_,SD_BOTH);
  closesocket(s_);
}
//----< test stub >--------------------------------------------------

#ifdef TEST_SOCKETS
#include <iostream>

void main()
{
  /*
   * Note: 
   * - No threads are used in this test stub, so it doesn't represent typical usage.
   * - Neither do we do any message framing.  So correct operation depends on lightly
   *   loaded receiver.
   * - We will provide a simple demo using threads and queues with message framing
   *   to show how that is done in another demo.  
   */
  std::cout << "\n  Testing Socket, SocketListener, and SocketSystem classes";
  std::cout << "\n ==========================================================\n";
  try
  {
    SocketSystem su;
    std::string host = su.getHostName();
    std::cout << "\n  host machine name:           " << host.c_str();
    std::string ip = su.getIpFromName(host);
    std::cout << "\n  IP Address of machine:       " << ip.c_str();
    std::string name = su.getNameFromIp(ip);
    std::cout << "\n  DNS name of machine from ip: " << name.c_str() << '\n';
  
    std::cout << "\n  Client connecting to Server";
    std::cout << "\n -----------------------------";
    SocketListener listener(2048);
    Socket sendr;
    if(!sendr.connect("127.0.0.1",2048))  // this works too
    //if(!sendr.connect("Apocalypse",2048))  // can use ip addr, e.g., 127.0.0.1
    {
      std::cout << "\n connection failed\n\n";
      return;
    }
    Socket recvr = listener.waitForConnect();
    std::cout << "\n  remote ip is: " << recvr.System().getRemoteIP(&recvr);
    std::cout << ", remote port is: " << recvr.System().getRemotePort(&recvr);
    std::cout << "\n  local ip is:  " << recvr.System().getLocalIP();
    std::cout << ", local port is: " << recvr.System().getLocalPort(&recvr) << '\n';

    std::cout << "\n  sending from Client to Server";
    std::cout << "\n -------------------------------";

    // sending
    const char* msg1 = "this is a message";
    sendr.sendAll(msg1,strlen(msg1)+1);

    const char* msg2 = "and a final message";
    sendr.sendAll(msg2,strlen(msg2)+1);

    sendr.sendAll("quit",strlen("quit")+1);

    // receiving
    std::cout.flush();
    const int BufLen = 256;
    char buffer[BufLen];

    std::cout << "\n  " << recvr.bytesLeft() << " bytes ready for reading";
    recvr.recvAll(buffer,strlen(msg1)+1);
    std::cout << "\n  Server received: " << buffer;
    std::cout << "\n  " << recvr.bytesLeft() << " bytes ready for reading";

    recvr.recvAll(buffer,strlen(msg2)+1);
    std::cout << "\n  Server received: " << buffer;
    std::cout << "\n  " << recvr.bytesLeft() << " bytes ready for reading";

    recvr.recvAll(buffer,strlen("quit")+1);
    std::cout << "\n  Server received: " << buffer;
    std::cout << "\n  " << recvr.bytesLeft() << " bytes ready for reading";
    std::cout << "\n";

    std::cout << "\n  Breaking connection";
    recvr.disconnect();
    sendr.disconnect();

    // new connection

    std::cout << "\n  Establishing new connection\n";
    if(!sendr.connect("127.0.0.1",2048))
    {
      throw std::exception("\n  reconnect failed");
    }
    recvr = listener.waitForConnect();
    msg1 = "another message after reconnecting";
    std::cout << "\n  Client sending: " << msg1;
    sendr.WriteLine(msg1);
    std::string temp = recvr.ReadLine();
    std::cout << "\n  Server received: " << temp;
    std::cout << "\n";

    // demonstrating full duplex operation
    std::cout << "\n  sending from Server back to Client";
    std::cout << "\n ------------------------------------";

    // sending
    msg1 = "sending message back";
    std::cout << "\n  Server sending message: " << msg1;
    recvr.WriteLine(msg1);
    std::cout << "\n  Server sending message: " << "quit";
    recvr.WriteLine("quit");

    // receiving
    std::cout << "\n  Client received: " << sendr.ReadLine();
    std::cout << "\n  Client received: " << sendr.ReadLine();
    std::cout << "\n  Client received: " << sendr.ReadLine();
    std::cout << std::endl;

    // copy construction
    Socket sendrCopy = sendr;
    Socket recvrCopy = recvr;

    // copy construction
    std::cout << "\n  sending and recieving with socket copies";
    std::cout << "\n ------------------------------------------";

    sendrCopy.WriteLine("string from sendrCopy");
    // recieving with copy
    std::cout << "\n  recvrCopy received: " << recvrCopy.ReadLine();
    std::cout << std::endl;

    // socket assignment
    sendr = sendrCopy;
    recvr = recvrCopy;

    // sending from assigned socket
    std::cout << "\n  sending and recieving with assigned sockets";
    std::cout << "\n ---------------------------------------------";

    sendr.WriteLine("string from AssignedSendr");
    // recieving with copy
    std::cout << "\n  AssignedRecvr received: " << recvr.ReadLine();
    std::cout << std::endl;

    sendr.disconnect();
    recvr.disconnect();
  }
  catch(std::exception& e)
  {
    std::cout << "\n  " << e.what() << "\n\n";
  }
  std::cout << "\n\n";
}

#endif
