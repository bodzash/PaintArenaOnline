#pragma once

#include <cmath>

void NormalizeVector(float& x, float& y)
{
  float m = std::max(sqrtf(x * x + y * y), 1.0f);
  x /= m;
  y /= m;
}

float Approach(float Start, float End, float Shift)
{
  if (Start < End)
  {
    return std::min(Start + Shift, End);
  }
  else
  {
    return std::max(Start - Shift, End);
  }
}

int RandomRange(int Min, int Max)
{
  return (std::rand() % (Max - Min)) + Min;
}

// 0 - 10

// 5 - 15 ->  