


///////////////////////////////////////////////////////////////
// ChronoTimer.h - uses Chrono::high_resolution_clock        //
//                                                           //
// Platfrom:                                                 //
// Application:  Project #2 help,                            //
//               CSE687 Object Oriented Design, Spring 2014  //
// Author:       Jim Fawcett, CST 4-187, Syracuse University //
///////////////////////////////////////////////////////////////
/*
  Package Operations:
  -------------------
  Class Timer provides stop watch functionality with start(), stop(),
  and elapsedTime<T>(), where T can be any one of the following:
  std::chrono::minutes
  std::chrono::seconds
  std::chrono::milliseconds
  std::chrono::microseconds
  std::chrono::nanoseconds
  The return value of elapsedTime<T>() is an integer value representing
  the elapsed time in the specified precision.

  Required files:
  ---------------
  ChronoTimer.h, ChronoTimer.cpp

  Build command:
  --------------
  devenv ChronoTimer.sln /rebuild debug

  Maintanence History:
  --------------------
  ver 1.0 : 23 Jan 14
  - first release
*/

#include <chrono>

class Timer
{
public:
  Timer() {}
  void start();
  void stop();
  template <typename T>  // T is time unit, e.g., std::chrono::microsecond
  __int64 elapsedTime();
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> tstart;
  std::chrono::time_point<std::chrono::high_resolution_clock> tstop;
};

inline void Timer::start() 
{
  tstart = std::chrono::high_resolution_clock::now();
}

inline void Timer::stop()
{
  tstop = std::chrono::high_resolution_clock::now();
}

//----< returns number of time units defined by T >------------------

template <typename T>
__int64 Timer::elapsedTime()
{
  return std::chrono::duration_cast<T>(tstop - tstart).count();
}