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
  return (rand() % (Max - Min)) + Min;
}

int RandomInt(int Max)
{
  return rand() % Max;
}

float PointDistance(float x1, float y1, float x2, float y2)
{
  return sqrtf(pow(x2 - x1, 2.0f) + pow(y2 - y1, 2.0f));
}

float PointDirection(float x1, float y1, float x2, float y2)
{
  float Theta = atan2(y2 - y1, x2 - x1);
  if (Theta < 0) Theta += 6.28f;
  return Theta;
}