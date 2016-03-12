#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H
///////////////////////////////////////////////////////////////////
// BlockingQueue.h - Thread-safe queue that blocks on empty deQ  //
// ver 2.0                                                       //
// Language: standard C++                                        //
// Platform: Dell Dimension T7400, Windows 7, SP #1              //
// Application: Resource for DO projects                         //
// Author:      Jim Fawcett, CST 4-187, Syracuse University      //
//              jfawcett@twcny.rr.com, (315) 443-3948            //
// Student:     Yufan Gong                                       //
//              ygong01@syr.edu                                  //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package contains a BlockingQueue<> class that is thread-safe
* and blocks a deQ'er if the queue is empty.  Blocking is accomplished
* using a ConditionVariable (introduced with Vista) and an associated
* lock based on a CriticalSection.
*
* Users don't need to be aware of how this works.  They just use
* the queue without worrying about locking.
*
* Public Interface:
* =================
* enQ
* deQ
* size
*
* Required Files:
* ---------------
* BlockingQueue.h, BlockingQueue.cpp
*
* Build Process:
* --------------
* cl /EHa /DTEST_BLOCKINGQUEUE BlockingQueue.cpp
*
* Maintenance History:
* --------------------
* ver 2.0 : 12 Apr 14
* - updated to c++11 threads
* ver 1.1 : 24 Mar 13
* - small revisions for new threadBase
* ver 1.0 : 19 Feb 12
* - first release
*/

#include <queue>
//#include <Windows.h>
#include <condition_variable>
#include <mutex>
#include <thread>
///////////////////////////////////////////////////////////////
// BlockingQueue<Msg>

template <typename Msg>
class BlockingQueue
{
public:
	BlockingQueue();
	void enQ(Msg msg);
	Msg deQ();
	size_t size();
private:
	std::queue<Msg> q_;
	std::mutex mtx_;
	std::condition_variable cv_;
};

//----< Ctor >-------------------------------------------------

template <typename Msg>
BlockingQueue<Msg>::BlockingQueue()
{
	cv_.notify_all();  // clear lock - probably not necessary
}

//----< add a message to queue >-------------------------------

template <typename Msg>
void BlockingQueue<Msg>::enQ(Msg msg)
{
	{
		std::lock_guard<std::mutex> l(mtx_);
		q_.push(msg);
	}
	cv_.notify_one();
}
//----< remove a message from queue >--------------------------

template <typename Msg>
Msg BlockingQueue<Msg>::deQ()
{
	std::unique_lock<std::mutex> l(mtx_);
	if (q_.size() > 0)
	{
		Msg temp = q_.front();
		q_.pop();
		return temp;
	}
	// may have spurious returns so loop on !condition

	while (q_.size() == 0)
		cv_.wait(l, [this]() { return q_.size() > 0; });
	Msg temp = q_.front();
	q_.pop();
	return temp;
}
//----< return number of queueud messages >--------------------

template <typename Msg>
size_t BlockingQueue<Msg>::size()
{
	std::lock_guard<std::mutex> l(mtx_);
	return q_.size();
}

#endif
