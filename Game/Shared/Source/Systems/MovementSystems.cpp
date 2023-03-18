#include "Systems/MovementSystems.hpp"

void DynamicMovementSystem(float DeltaTime, entt::registry& Scene)
{
  auto View = Scene.view<Position, Velocity, Speed>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Vel = View.get<Velocity>(Entity);
    auto& Spd = View.get<Speed>(Entity);

    float VelocityDirX = Vel.x;
    float VelocityDirY = Vel.y;

    NormalizeVector(VelocityDirX, VelocityDirY);

    Pos.x += VelocityDirX * abs(Vel.x) * DeltaTime;
    Pos.y += VelocityDirY * abs(Vel.y) * DeltaTime;

    Vel.x = Approach(Vel.x, 0.0f, Spd.Deceleration);
    Vel.y = Approach(Vel.y, 0.0f, Spd.Deceleration);
  }
}

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