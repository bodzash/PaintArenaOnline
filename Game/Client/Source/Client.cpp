#include <iostream>
#include <string>

#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Network.hpp"
#include "Components.hpp"

using std::string;

void SpriteRendererSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);

    DrawCircle(Pos.x, Pos.y, 16.0f, RED);
  }
}

void HealthRendererSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position, Health>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Hel = View.get<Health>(Entity);

    DrawText(std::to_string(Hel.Current).c_str(), Pos.x, Pos.y, 32, BLACK);
  }
}

int main(void)
{
  entt::registry Scene;

  InitWindow(640, 480, "Marble Shooter");
  SetTargetFPS(60);

  ConnectToServer();
  
  while (!WindowShouldClose())
  {
    PollNetwork(Scene);

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
      SendMovement(IsKeyDown(KEY_A), IsKeyDown(KEY_D),
        IsKeyDown(KEY_W), IsKeyDown(KEY_S));
    }

    BeginDrawing();
    ClearBackground({167, 167, 167, 255});
    SpriteRendererSystem(Scene);
    HealthRendererSystem(Scene);
    EndDrawing();
  }

  DeInitENet();
  CloseWindow();
}