#pragma once

#include <cstdint>

#include "enet/enet.h"
#include "entt/entity/registry.hpp"

// Misc
struct RemotePeer
{
  entt::entity Id = (entt::entity)0;
  uint8_t NetworkId = 0;
  bool bActive = false;
  ENetPeer* pPeer = nullptr;
};

// Client To Server Messages
struct ClientMovementCommands
{
  uint8_t Type = 0;
  bool bLeft;
  bool bRight;
  bool bUp;
  bool bDown;
};

struct ClientShootingCommands
{
  uint8_t Type = 0;
  float Angle;
};

// Server To Client Messages
struct OnConnection
{
  uint8_t Type = 0;
  uint8_t Nid;
};

struct CreatePlayer
{
  uint8_t Type = 1;
  uint8_t Nid;
  int Health;
  float x;
  float y;
};

struct DeletePlayer
{
  uint8_t Type = 2;
  uint8_t Nid;
};

struct UpdatePlayer
{
  uint8_t Type = 3;
  uint8_t Nid;
  int Health;
  float x;
  float y;
};

struct CreateBullet
{
  uint8_t Type = 4;
  uint8_t Nid;
  float x;
  float y;
  float Direction;
};

struct DeathPlayer
{
  uint8_t Type = 5;
  uint8_t Nid;
  float x;
  float y;
};