#pragma once

#include <string>

// BIG TODO RENAME ALL OF THESE TO ...Component

// TODO PositionComponent
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

// TODO NetworkComponent
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
  std::string Asset;
};

struct Direction
{
  float Angle = 0.0f;
};

struct Collider
{
  float Diameter;
};

struct PlayerInput
{
  bool bLeft;
  bool bRight;
  bool bUp;
  bool bDown;
};

struct PlayerTag{};
struct BulletTag{};
struct TileTag{};