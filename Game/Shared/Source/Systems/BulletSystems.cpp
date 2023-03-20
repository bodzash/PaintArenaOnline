#include "Systems/BulletSystems.hpp"

void ClientBulletDamageSystem(entt::registry& Scene)
{
  auto Bullets = Scene.view<BulletTag, TeamId, Position, Collider, Shadow>();
  auto Players = Scene.view<PlayerTag, TeamId, Position, Collider, Shake, Audio>();
  for (auto Player : Players)
  {
    // Player
    auto& PTid = Players.get<TeamId>(Player);
    auto& PPos = Players.get<Position>(Player);
    auto& PCol = Players.get<Collider>(Player);
    auto& PAud = Players.get<Audio>(Player);
    auto& PShk = Players.get<Shake>(Player);
    
    for (auto Bullet : Bullets)
    {
      // Bullet
      auto& BTid = Bullets.get<TeamId>(Bullet);
      auto& BPos = Bullets.get<Position>(Bullet);
      auto& BCol = Bullets.get<Collider>(Bullet);
      auto& BShd = Bullets.get<Shadow>(Bullet);

      // Check if own or Dead
      if (PTid.Team == BTid.Team) break;

      // Check for collision
      if ((PCol.Radius + BCol.Radius) > PointDistance(PPos.x, PPos.y, BPos.x, BPos.y))
      {
        PShk.Amount = 2.4f;
        PAud.bIsPlaying = true;
        Scene.destroy((entt::entity)BShd.Reference);
        Scene.destroy(Bullet);
      }
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
      auto& BTid = Bullets.get<TeamId>(Bullet);
      auto& BPos = Bullets.get<Position>(Bullet);
      auto& BCol = Bullets.get<Collider>(Bullet);

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