#ifndef ITASK_H
#define ITASK_H
///////////////////////////////////////////////////////////////
// ITask.cpp - Abstract Interface for long task              //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
// Changed by Yufan Gong for OOD project4, Apr 29, 2014      //
///////////////////////////////////////////////////////////////


using namespace System;
using namespace System::Text;
#include <string>

public ref class ITask abstract
{
public:
  delegate void EventCallBack(String^);  // callback status to MainWindow
  EventCallBack^ callback = nullptr;
  
  // UI thread sends its dispatcher to Bridge for callback
  virtual void run(System::Windows::Threading::Dispatcher^ disp, String^ Command, String^ Path,	String^ Content) = 0;
  // override function to handle text search thread input
  virtual void runSearch(System::Windows::Threading::Dispatcher^ disp, String^ Command, String^ path, String^ Content, int ThreadNumber) = 0;
  // callback for native task to return progress count to Bridge

  virtual void doCallBack(String^ reply) = 0;
  
  static ITask^ Create(String^ ip, int port);  // factory function

};

#endif
