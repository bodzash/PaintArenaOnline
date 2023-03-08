#include <iostream>
#include <string>
#include <map>

#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Network.hpp"
#include "Components.hpp"

using std::string;

struct SpriteAsset
{
  float x = 0.0f;
  float y = 0.0f;
  float Width = 8.0f;
  float Height = 8.0f;
};

void SpriteRendererSystem(entt::registry& Scene, Texture2D Tex,
  std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<Position, Sprite>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Spr = View.get<Sprite>(Entity);

    DrawTextureRec(Tex, {Assets[Spr.Asset].x, Assets[Spr.Asset].y,
      Assets[Spr.Asset].Width, Assets[Spr.Asset].Height},
      {Pos.x, Pos.y}, WHITE);
  }
}

void HealthRendererSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position, Health>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Hel = View.get<Health>(Entity);

    DrawText(std::to_string(Hel.Current).c_str(), Pos.x, Pos.y, 24, BLACK);
  }
}

int main(void)
{
  entt::registry Scene;

  InitWindow(640, 480, "Marble Shooter");
  SetTargetFPS(60);

  Texture2D TextureAtlas = LoadTexture("./Resources/SpriteAtlas.png");
  std::map<string, SpriteAsset> TextureAssets;

  // Misc
  TextureAssets["FloorTile"] = {0, 12, 16, 16};
  TextureAssets["PlayerShadow"] = {24, 11, 8, 3};
  TextureAssets["BulletShadow"] = {32, 11, 4, 3};

  // Players
  TextureAssets["PinkPlayer"] = {0, 0, 8, 8};
  TextureAssets["GreenPlayer"] = {8, 0, 8, 8};
  TextureAssets["RedPlayer"] = {16, 0, 8, 8};
  TextureAssets["CyanPlayer"] = {24, 0, 8, 8};
  TextureAssets["LimePlayer"] = {32, 0, 8, 8};
  TextureAssets["OrangePlayer"] = {40, 0, 8, 8};


  // Bullets
  TextureAssets["PinkBullet"] = {0, 8, 4, 4};
  TextureAssets["GreenBullet"] = {4, 8, 4, 4};
  TextureAssets["RedBullet"] = {8, 8, 4, 4};
  TextureAssets["CyanBullet"] = {12, 8, 4, 4};
  TextureAssets["LimeBullet"] = {16, 8, 4, 4};
  TextureAssets["OrangeBullet"] = {20, 8, 4, 4};

  ConnectToServer();
  
  while (!WindowShouldClose())
  {
    PollNetwork(Scene);

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
      SendMovement(IsKeyDown(KEY_A), IsKeyDown(KEY_D),
        IsKeyDown(KEY_W), IsKeyDown(KEY_S));
    }

    if (IsKeyDown(KEY_F)) Disconnect();

    BeginDrawing();
    ClearBackground({167, 167, 167, 255});
    SpriteRendererSystem(Scene, TextureAtlas, TextureAssets);
    HealthRendererSystem(Scene);
    DrawText(std::to_string(IsConnected() ? 1 : 0).c_str(), 8, 8, 24, BLUE);
    EndDrawing();
  }

  DeInitENet();
  CloseWindow();
}