#include <Math.hpp>

// Vector
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

float Lerp(float First, float Second, float Amount)
{
  return First + (Second - First) * Amount;
}

// Random
int RandomRange(int Min, int Max)
{
  return (rand() % (Max - Min)) + Min;
}

float RandomRange(float Min, float Max)
{
  return (Max - Min) * ((float)(rand()) / (float)(RAND_MAX)) + Min;
}

int RandomInt(int Max)
{
  return rand() % Max;
}

// Trigonometry
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

/*
template<typename T>
T ChooseOne(T Option)
{
  return Option;
}
*/