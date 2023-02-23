#include <chrono>
#include <iostream>
#include "mingw.thread.h"

int main()
{
  using clock = std::chrono::steady_clock;

  auto next_frame = clock::now();

  while(true)
  {
    next_frame += std::chrono::milliseconds(1000 / 5); // 5Hz

    // do stuff
    std::cout << std::time(0) << '\n'; // 5 for each second

    // wait for end of frame
    std::this_thread::sleep_until(next_frame);
    
  }
}