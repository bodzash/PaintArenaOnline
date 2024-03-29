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
  int Depth;
  int Color;
  float OffsetX = 0.0f;
  float OffsetY = 0.0f;
  float OgOffsetX = OffsetX;
  float OgOffsetY = OffsetY;
};

struct Audio
{
  std::string Asset1;
  std::string Asset2;
  bool bIsPlaying = false;
  bool bOneTime = true;
};

struct Smudge
{
  std::string Asset;
  int Color;
};

struct SmudgeBall
{
  float Height;
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

struct NetworkId
{
  uint8_t Id;
};

struct Health
{
  int Min = 0;
  int Max = 100;
  int Current = Max;
};

struct Shadow
{
  uint32_t Reference;
};

struct Shake
{
  float Amount = 0.0f;
};

struct Direction
{
  float Angle = 0.0f;
};

struct Collider
{
  float Diameter = 1.0f;
  float Radius = Diameter / 2.0f;
};

struct PlayerInput
{
  bool bLeft;
  bool bRight;
  bool bUp;
  bool bDown;
};

struct TeamId
{
  uint8_t Team;
};

struct PlayerTag{};
struct BulletTag{};