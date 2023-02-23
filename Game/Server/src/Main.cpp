#include <chrono>
#include <iostream>
#include "mingw.thread.h"
#include <cstdint>

std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

const float FrameRate = 1000 / 60;
float DeltaTime = 0.016f;

int main()
{
  while (true)
  {
    // Maintain 60 FPS
    a = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> work_time = a - b;

    if (work_time.count() < FrameRate)
    {
      std::chrono::duration<double, std::milli> delta_ms(FrameRate - work_time.count());
      std::chrono::milliseconds delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
      std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
    }

    b = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> sleep_time = b - a;

    float DeltaTime = 1 / (1000 / (work_time + sleep_time).count());

    // Network polling
    // Update

    std::cout << DeltaTime << "\n";
  }
}

/*
int main()
{
  using clock = std::chrono::steady_clock;
  using time_point = std::chrono::_V2::steady_clock::time_point;

  time_point next_frame = clock::now();

  while(true)
  {
    // Time Start bullshit
    next_frame += std::chrono::milliseconds(1000 / 60);

    // Network Poll
    // Update

    std::cout << 0.0016f << '\n';

    // End of Frame
    std::this_thread::sleep_until(next_frame);
  }
}
*/