#include <iostream>
#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Net.hpp"

struct Position
{
  float x = 0.0f;
  float y = 0.0f;
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

struct NetId
{
  int Id;
};

void SpriteRendererSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);

    DrawCircle(Pos.x, Pos.y, 16.0f, RED);
  }
}

int main(void)
{
  entt::registry Scene;

  entt::entity Player = Scene.create();
  Scene.emplace<NetId>(Player, 0);
  Scene.emplace<Position>(Player, 0, 0);

  InitWindow(640, 480, "Game Client");
  SetTargetFPS(60);

  ConnectToServer();
  
  while (!WindowShouldClose())
  {
    PollNet(Scene);

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
      SendMovement(IsKeyDown(KEY_A), IsKeyDown(KEY_D),
        IsKeyDown(KEY_W), IsKeyDown(KEY_S));
    }

    BeginDrawing();
    ClearBackground({167, 167, 167, 255});

    SpriteRendererSystem(Scene);
    
    EndDrawing();
  }
  CloseWindow();
}