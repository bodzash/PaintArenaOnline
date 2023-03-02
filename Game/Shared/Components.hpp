#pragma once


struct Position
{
  float x = 0.0f;
  float y = 0.0f;
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
  int Id;
};

struct PlayerInput
{
  bool bLeft;
  bool bRight;
  bool bUp;
  bool bDown;
};