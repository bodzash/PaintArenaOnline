#include "Systems/BulletSystems.hpp"

// TODO move this function to some physicsdynamicssystems.hpp/cpp
void DirectionalMovementSystem(entt::registry& Scene, float Delta)
{
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

void ServerBulletDamageSystem(entt::registry& Scene)
{
  auto Bullets = Scene.view<BulletTag, TeamId, Position, Collider>();
  auto Players = Scene.view<PlayerTag, TeamId, Position, Collider, Health>();
  for (auto Player : Players)
  {
    // Player
    auto& PTid = Players.get<TeamId>(Player);
    auto& PPos = Players.get<Position>(Player);
    auto& PCol = Players.get<Collider>(Player);
    auto& PHel = Players.get<Health>(Player);
    
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
        PHel.Current -= 20;
        Scene.destroy(Bullet);
      }
    }
  }
}