///////////////////////////////////////////////////////////////
// Test.cs - startup multiple concurrent RequestProcessors   //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
// Modified by Yufan Gong                                    //
///////////////////////////////////////////////////////////////
/*
 * 
 * added an argument list to store some argument for run serveral clients
 * to process different command concurrently.
 * 
 */ 

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;

namespace Tester
{
  class TestSpawn
  {
    object synch = new object();
    List<string> Results = new List<string>();
    string path = ".";

    //----< return List of test results >------------------------------

    List<string> getResults() { return Results; }

    //----< is this the right directory? >-----------------------------

    void showFilesOnPath()
    {
      Console.Write("\n  {0}", Directory.GetCurrentDirectory());
      string[] files = Directory.GetFiles(path);
      foreach (string file in files)
        Console.Write("\n  {0}", file);
      Console.Write("\n");
    }
    //----< Run RequestProcessor showing its window >------------------

    void startTestInWindow(String argument)
    {
      ProcessStartInfo pInfo = new ProcessStartInfo();
      pInfo.FileName = path + "\\RequestProcessor.exe";
      pInfo.Arguments = argument;
      Process rpProcess = Process.Start(pInfo);
      rpProcess.WaitForExit();
      rpProcess.Dispose();
    }
    //----< Run RequestProcessor windowless >--------------------------

    void startTestNoWindow(String argument)
    {
      ProcessStartInfo pInfo = new ProcessStartInfo();
      pInfo.FileName = path + "\\RequestProcessor.exe";
      pInfo.Arguments = argument;
      pInfo.UseShellExecute = false;
      pInfo.CreateNoWindow = true;
      pInfo.RedirectStandardOutput = true;
      using (Process rpProcess = Process.Start(pInfo))
      {
        using (StreamReader rdr = rpProcess.StandardOutput)
        {
          while(true)
          {
            if (rdr.EndOfStream)
              break;
            string result = rdr.ReadToEnd();
            lock(synch)
            {
              Results.Add(result);
            }
            //Console.Write("\n  {0}", result);
          }
        }
      }
    }
    //----< process>--------------------------

    void processing(string[] args)
    {
      Console.Write("\n  Tester running RequestProcess");
      Console.Write("\n ===============================\n");
      if(args.Count() == 0)
      {
        Console.Write("\n\n  Please enter on command line number of RequestProcessors to Start\n\n");
        return;
      }
      String argument1 = "127.0.0.1 10000 TextSearch ../ \"*.cpp *.h *.cs\" /s 9 text";
      String argument2 = "127.0.0.1 10001 Similarity ../ \"*.cpp *.h *.cs\" \"/s /b /r\"";
      String argument3 = "127.0.0.1 10002 Files ../ *.* /s";
      String argument4 = "127.0.0.1 10003 Upload ../Client/Upload/Message.h Message.h";
      String argument5 = "127.0.0.1 10004 Download ../Server/Repository/Message.cpp Message.cpp";
      List<string> arguments = new List<string>();
      arguments.Add(argument1);
      arguments.Add(argument2);
      arguments.Add(argument3);
      arguments.Add(argument4);
      arguments.Add(argument5);

      int numToStart = Int32.Parse(args[0]);
      List<Task> taskList = new List<Task>();

      Stopwatch timer = new Stopwatch();
      timer.Start();

      for (int i = 0; i < numToStart; ++i )
      {
          int j = i % 5;
          taskList.Add(Task.Run(() => startTestNoWindow(arguments[j])));
      }

      foreach (Task task in taskList)
        task.Wait();

      timer.Stop();
      long time = timer.ElapsedMilliseconds;
      long average = time / numToStart;
      
      List<string> Results = getResults();
      foreach (string result in Results)
      Console.Write(result);
      Console.Write("\n  Test complete\n");
      Console.Write("\n Processing time : {0}", time);
      Console.Write("\n Average time : {0}", average);
    }
    //----< Run Tests >------------------------------------------------

    static void Main(string[] args)
    {
      TestSpawn ts = new TestSpawn();
      ts.processing(args);
      while (true) ;

    }
  }
}
