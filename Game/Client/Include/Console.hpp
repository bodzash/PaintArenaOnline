#pragma once

#include <iostream>
#include <string>
#include <list>

using std::string;
using std::list;

#define ALL 0
#define TRACE 1
#define DEBUG 2
#define INFO 3
#define WARN 4
#define ERROR 5
#define FATAL 6

struct ConsoleMessage
{
  int Level;
  string Message;
};

class Console
{
public:
  Console();
  void Log(string Message);
  void Log(int Level, string Message);
  void ClearLogs();
  list<ConsoleMessage> GetLogs();

private:
  bool mbPrintToFile;
  list<ConsoleMessage> mLogList;
};

// Define Global Console
extern Console GCon;