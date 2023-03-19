#include "Timing.hpp"

const float FrameRate = 1000 / 60;

system_clock::time_point TimeStart = system_clock::now();
system_clock::time_point TimeEnd = system_clock::now();

float CalculateDeltaTimeAndSleepThread()
{
  TimeStart = system_clock::now();
  duration<float, std::milli> WorkTime = TimeStart - TimeEnd;

  if (WorkTime.count() < FrameRate)
  {
    duration<float, std::milli> DeltaMs(FrameRate - WorkTime.count());
    milliseconds DeltaMsDuration = duration_cast<milliseconds>(DeltaMs);
    std::this_thread::sleep_for(milliseconds(DeltaMsDuration.count()));
  }

  TimeEnd = system_clock::now();
  duration<float, std::milli> sleep_time = TimeEnd - TimeStart;

  return (1.0f / (1000.0f / (WorkTime + sleep_time).count()));
}