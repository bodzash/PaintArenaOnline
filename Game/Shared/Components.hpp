#pragma once

#include <string>

struct Position
{
  float x = 0.0f;
  float y = 0.0f;
};

struct Sprite
{
  std::string Asset;
};

struct Velocity
{
  float x = 0.0f;
  float y = 0.0f;
};

struct Speed
{
  float MaxSpeed = 0.0f;
  float Acceleration = 0.0f;
  float Deceleration = 0.0f;
};

struct NetId
{
  uint8_t Id;
};

struct Health
{
  int Min = 0;
  int Max = 100;
  int Current = Max;
};

struct PlayerInput
{
  bool bLeft;
  bool bRight;
  bool bUp;
  bool bDown;
};