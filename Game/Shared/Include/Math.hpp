#pragma once

#include <cmath>
#include <ctime>

void NormalizeVector(float& x, float& y);

float Approach(float Start, float End, float Shift);

float Lerp(float First, float Second, float Amount);

void SetRandomSeed();
int RandomRange(int Min, int Max);
float RandomRange(float Min, float Max);
int RandomInt(int Max);

float PointDistance(float x1, float y1, float x2, float y2);
float PointDirection(float x1, float y1, float x2, float y2);