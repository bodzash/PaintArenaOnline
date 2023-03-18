#include "Console.hpp"

Console::Console()
{
}

void Console::Log(string Message)
{
  std::cout << Message << "\n";
  mLogList.emplace_back<ConsoleMessage>({ALL, Message});
}

void Console::Log(int Level, string Message)
{
  mLogList.emplace_back<ConsoleMessage>({Level, Message});
}

void Console::ClearLogs()
{
  mLogList.clear();
}

std::list<ConsoleMessage> Console::GetLogs()
{
  return mLogList;
}

// Implement Global Console
Console GCon;