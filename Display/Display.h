/////////////////////////////////////////////////////////////////////////////
// Display.h -  Display package used to print output to the screen.        //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      1.1													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This package used to print message content to the screen.
*
* Public Interface:
* =================
* void Title(std::string option);
* void ShowMessage(size_t require, ServiceMessage message);
*
* Build Command:   cl /EHa /DTEST_DISPLAY display.cpp
* ===========================================================
*
* Maintenance History:
* ====================
* ver 1.1 : Apr 29, 2014
* - add some help function
* ver 1.0 : 12 Apr 14
* - first release
*/


#ifndef DISPLAY_H
#define DISPLAY_H


#include <iostream>
#include <iomanip>
#include <string>
#include <mutex>
#include "../Message/Message.h"

///////////////////////////////////////////////////////////////
// Display

class Display
{
public:
	void Title(std::string option);
	void ShowMessage(size_t require, ServiceMessage message);
	void ShowServerRecieve(int i);
	void ShowServerReply(int i);
	void ShowClientRequest(int i);
	void ShowClientRecieve(int i);
private:
	void PrintMessage(ServiceMessage message);
	void PrintMsgBody(ServiceMessage message);
	//std::mutex mtx_;
};
#endif

