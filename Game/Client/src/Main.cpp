#include "raylib.h"
#include "entt/entt.hpp"
#include "Net.h"
#include <iostream>

struct PositionComponent
{
  int x, y = 0;
};

void RendererSystem(entt::registry &Reg)
{
  auto View = Reg.view<PositionComponent>();

  for(auto Entity: View)
  {
    auto& Pos = View.get<PositionComponent>(Entity);
    DrawCircle(Pos.x, Pos.y, 32.0f, WHITE);
  }
}

entt::registry Registry;

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  SetTargetFPS(60);

  Connect();

  auto Player = Registry.create();
  Registry.emplace<PositionComponent>(Player, 32, 44);
  
  while (!WindowShouldClose())
  {
    PollNet();
    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
      RendererSystem(Registry);
    EndDrawing();
  }
  CloseWindow();
  
  return 0;
}
