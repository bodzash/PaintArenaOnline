#pragma once

#include "entt/entity/registry.hpp"

#include "Components.hpp"
#include "Math.hpp"

entt::entity CreatePrefabPlayer(entt::registry& Scene, uint8_t NetId)
{
  entt::entity Player = Scene.create();
  Scene.emplace<PlayerTag>(Player);
  Scene.emplace<TeamId>(Player, NetId);
  Scene.emplace<NetworkId>(Player, NetId);
  Scene.emplace<Position>(Player, (float)RandomRange(4, 236), (float)RandomRange(4, 172));
  Scene.emplace<Velocity>(Player);
  Scene.emplace<Speed>(Player, 140.0f, 45.0f, 27.0f);
  Scene.emplace<Collider>(Player, 8.0f);
  Scene.emplace<Health>(Player, 0, 100);
  Scene.emplace<PlayerInput>(Player);

  return Player;
}

entt::entity CreatePrefabBullet(entt::registry& Scene, float x, float y, float Angle,
  uint8_t NetId)
{
  entt::entity Bullet = Scene.create();
  Scene.emplace<BulletTag>(Bullet);
  Scene.emplace<TeamId>(Bullet, NetId);
  Scene.emplace<Position>(Bullet, x, y);
  Scene.emplace<Direction>(Bullet, Angle);
  Scene.emplace<Collider>(Bullet, 4.0f);
  Scene.emplace<Speed>(Bullet, 140.0f);

  return Bullet;
}