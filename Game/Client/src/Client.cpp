#include <iostream>
#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Net.h"

entt::registry Registry;

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  SetTargetFPS(60);

  Connect();
  
  while (!WindowShouldClose())
  {
    PollNet();

    if (IsKeyPressed(KEY_SPACE)) SendPacket();

    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
  }
  CloseWindow();
  
  return 0;
}
