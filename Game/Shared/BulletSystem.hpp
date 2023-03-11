#pragma once

#include "entt/entity/registry.hpp"

#include "Components.hpp"
#include "Math.hpp"

void BulletMovementSystem(entt::registry& Scene, float Delta)
{
  //BulletTag
  auto View = Scene.view<Position, Direction, Speed>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Dir = View.get<Direction>(Entity);
    auto& Spd = View.get<Speed>(Entity);

    // Move angle
    Pos.x += (Spd.MaxSpeed * (float)cos(Dir.Angle)) * Delta;
    Pos.y += (Spd.MaxSpeed * (float)sin(Dir.Angle)) * Delta;
  }
}

void RemoveBulletOutOfBoundsSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position, BulletTag>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);

    if (Pos.x < 0.0f)
    {
      Scene.destroy(Entity);
      break;
    }
    else if (Pos.x > 240.0f)
    {
      Scene.destroy(Entity);
      break;
    }

    if (Pos.y < 0.0f)
    {
      Scene.destroy(Entity);
      break;
    }
    else if (Pos.y > 176.0f)
    {
      Scene.destroy(Entity);
      break;
    } 
  }
}

void BulletDamageSystem(entt::registry& Scene, bool bCanDamage)
{
  auto Bullets = Scene.view<BulletTag, TeamId, Position, Collider>();
  auto Players = Scene.view<PlayerTag, TeamId, Position, Collider, Health, Audio>();
  for (auto Player : Players)
  {
    // Player
    auto& PTid = Players.get<TeamId>(Player);
    auto& PPos = Players.get<Position>(Player);
    auto& PCol = Players.get<Collider>(Player);
    auto& PHel = Players.get<Health>(Player);
    auto& PAud = Players.get<Audio>(Player);
    
    for (auto Bullet : Bullets)
    {
      // Bullet
      auto& BTid = Players.get<TeamId>(Bullet);
      auto& BPos = Players.get<Position>(Bullet);
      auto& BCol = Players.get<Collider>(Bullet);

      // Check if own or Dead
      if (PTid.Team == BTid.Team || PHel.Current <= PHel.Min) break;

      // Check for collision
      if ((PCol.Radius + BCol.Radius) > PointDistance(PPos.x, PPos.y, BPos.x, BPos.y))
      {
        if (bCanDamage) PHel.Current -= 20;
        PAud.bIsPlaying = true;
        Scene.destroy(Bullet);
      }
    }
  }
}