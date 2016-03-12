/////////////////////////////////////////////////////////////////////
// BlockingQueue.cpp - Thread-safe queue that blocks on empty deQ  //
// ver 2.0                                                         //
// Language: standard C++                                          //
// Platform: Dell Dimension T7400, Windows 7, SP #1                //
// Application: Resource for DO projects                           //
// Author:      Jim Fawcett, CST 4-187, Syracuse University        //
//              jfawcett@twcny.rr.com, (315) 443-3948              //
// Student:     Yufan Gong                                         //
//              ygong01@syr.edu                                    //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_BLOCKINGQUEUE

#include "BlockingQueue.h"
#include <string>
#include <iostream>
#include <sstream>

//----< here is where what you want to happen happens >--------

std::mutex ioLock;

void test(BlockingQueue<std::string>* pQ)
{
	std::string msg;
	do
	{
		msg = pQ->deQ();
		{
			std::lock_guard<std::mutex> l(ioLock);
			std::cout << "\n  thread deQed " << msg.c_str();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (msg != "quit");
}

//----< test stub >--------------------------------------------

int main()
{
	std::cout << "\n  Demonstrating C++11 Blocking Queue";
	std::cout << "\n ====================================";

	BlockingQueue<std::string> q;
	std::thread t(test, &q);

	for (int i = 0; i<25; ++i)
	{
		std::ostringstream temp;
		temp << i;
		std::string msg = std::string("msg#") + temp.str();
		{
			std::lock_guard<std::mutex> l(ioLock);
			std::cout << "\n   main enQing " << msg.c_str();
		}
		q.enQ(msg);
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}
	t.join();
}
#endif
