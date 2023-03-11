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
  float OffsetX = 0.0f;
  float OffsetY = 0.0f;
  float OgOffsetX = OffsetX;
  float OgOffsetY = OffsetY;
};

struct Audio
{
  std::string Asset;
  bool bIsPlaying = false;
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
  bool IsDead = false;
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

// TODO rename this pos
struct TeamId
{
  uint8_t Team;
};

struct PlayerTag{};
struct BulletTag{};
struct TileTag{};