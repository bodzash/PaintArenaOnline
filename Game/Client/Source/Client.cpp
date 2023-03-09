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

void ShadowRendererSystem(entt::registry& Scene, Texture2D Tex,
  std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<Position, Shadow>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Sha = View.get<Shadow>(Entity);

    DrawTextureRec(Tex, {Assets[Sha.Asset].x, Assets[Sha.Asset].y,
      Assets[Sha.Asset].Width, Assets[Sha.Asset].Height},
      {Pos.x + Sha.OffsetX, Pos.y + Sha.OffsetY}, {0, 0, 0, 100});
  }
}

void BackgroundRendererSystem(entt::registry& Scene, Texture2D Tex,
  std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<TileTag, Position>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);

    DrawTextureRec(Tex, {Assets["FloorTile"].x, Assets["FloorTile"].y,
      Assets["FloorTile"].Width, Assets["FloorTile"].Height},
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
  const int ScreenWidth = 160;
  const int ScreenHeight = 120;
  int ScreenWindowRatio = 6;

  InitWindow(ScreenWidth * ScreenWindowRatio, ScreenHeight * ScreenWindowRatio,
    "Marble Shooter");
  SetTargetFPS(60);

  entt::registry Scene;
  Camera2D MainCamera;
  Texture2D TextureAtlas = LoadTexture("./Resources/SpriteAtlas.png");
  std::map<string, SpriteAsset> TextureAssets;

  MainCamera.target = {0.0f, 0.0f};
  MainCamera.offset = {0.0f, 0.0f};
  MainCamera.rotation = 0.0f;
  MainCamera.zoom = (float)ScreenWindowRatio;

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

  // Fill background
  for(int i = 0; i <= 10; i++)
  {
    for(int j = 0; j <= 8; j++)
    {
      entt::entity Tile = Scene.create();
      Scene.emplace<Position>(Tile, 16.0f * i, 16.0f * j);
      Scene.emplace<TileTag>(Tile);
    }
  }

  ConnectToServer();
  
  while (!WindowShouldClose())
  {
    // Network stuff
    PollNetwork(Scene);

    // Need clean up
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
      SendMovement(IsKeyDown(KEY_A), IsKeyDown(KEY_D),
        IsKeyDown(KEY_W), IsKeyDown(KEY_S));
    }

    // Debug
    if (IsKeyPressed(KEY_KP_ADD)) MainCamera.zoom += 1.0f;
    if (IsKeyPressed(KEY_KP_SUBTRACT)) MainCamera.zoom -= 1.0f;

    // Update

    // Render
    BeginDrawing();
    ClearBackground({167, 167, 167, 255});
    
    BeginMode2D(MainCamera);

    BackgroundRendererSystem(Scene, TextureAtlas, TextureAssets);
    ShadowRendererSystem(Scene, TextureAtlas, TextureAssets);
    SpriteRendererSystem(Scene, TextureAtlas, TextureAssets);
    //HealthRendererSystem(Scene);

    EndMode2D();

    DrawText(std::to_string(IsConnected() ? 1 : 0).c_str(), 8, 8, 24, BLUE);

    EndDrawing();
  }

  Disconnect();
  DeInitENet();
  CloseWindow();
}