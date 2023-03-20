#include "Systems/BoundsSystems.hpp"

void RemoveBulletOutOfBoundsSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position, BulletTag>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    Shadow* Shd = Scene.try_get<Shadow>(Entity);

    if (Pos.x < 0.0f)
    {
      if (Shd != nullptr) Scene.destroy((entt::entity)Shd->Reference);
      Scene.destroy(Entity);
      break;
    }
    else if (Pos.x > 240.0f)
    {
      if (Shd != nullptr) Scene.destroy((entt::entity)Shd->Reference);
      Scene.destroy(Entity);
      break;
    }

    if (Pos.y < 0.0f)
    {
      if (Shd != nullptr) Scene.destroy((entt::entity)Shd->Reference);
      Scene.destroy(Entity);
      break;
    }
    else if (Pos.y > 176.0f)
    {
      if (Shd != nullptr) Scene.destroy((entt::entity)Shd->Reference);
      Scene.destroy(Entity);
      break;
    } 
  }
}

void KeepPlayerInBoundsSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position, PlayerTag>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);

    if (Pos.x < 4.0f)
    {
      Pos.x = 4.0f;
    }
    else if (Pos.x > 236.0f)
    {
      Pos.x = 236.0f;
    }

    if (Pos.y < 4.0f)
    {
      Pos.y = 4.0f;
    }
    else if (Pos.y > 172.0f)
    {
      Pos.y = 172.0f;
    }  
  }
}