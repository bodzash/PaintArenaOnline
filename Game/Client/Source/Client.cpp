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
  float OffsetX = 0.0f;
  float OffsetY = 0.0f;
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
      {Pos.x - Assets[Spr.Asset].OffsetX, Pos.y - Assets[Spr.Asset].OffsetY},
      WHITE);
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
      {Pos.x - Assets[Sha.Asset].OffsetX, Pos.y - Assets[Sha.Asset].OffsetY},
      {0, 0, 0, 100});
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

    DrawText(std::to_string(Pos.x).c_str(), Pos.x + 12, Pos.y, 8, BLACK);
    DrawText(std::to_string(Pos.y).c_str(), Pos.x + 12, Pos.y + 10, 8, BLACK);
  }
}

void ColliderDebugRendererSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position, Collider>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Col = View.get<Collider>(Entity);

    DrawCircleV({Pos.x, Pos.y}, Col.Diameter / 2.0f, {85, 170, 250, 191});
  }
}

int main(void)
{
  const int ScreenWidth = 224;
  const int ScreenHeight = 160;
  const int WindowWidth = 960;
  const int WindowHeight = 704;

  InitWindow(WindowWidth, WindowHeight, "Marble Shooter");
  SetTargetFPS(60);

  entt::registry Scene;
  Camera2D MainCamera;
  Texture2D TextureAtlas = LoadTexture("./Resources/SpriteAtlas.png");
  std::map<string, SpriteAsset> TextureAssets;

  MainCamera.target = {0.0f, 0.0f};
  MainCamera.offset = {0.0f, 0.0f};
  MainCamera.rotation = 0.0f;
  MainCamera.zoom = 4.0f;

  // Misc
  TextureAssets["FloorTile"] = {0, 12, 16, 16};
  TextureAssets["PlayerShadow"] = {24, 11, 8, 3, 4, -2};
  TextureAssets["BulletShadow"] = {32, 11, 4, 3, 2};

  // Players
  TextureAssets["PinkPlayer"] = {0, 0, 8, 8, 4, 4};
  TextureAssets["GreenPlayer"] = {8, 0, 8, 8, 4, 4};
  TextureAssets["RedPlayer"] = {16, 0, 8, 8, 4, 4};
  TextureAssets["CyanPlayer"] = {24, 0, 8, 8, 4, 4};
  TextureAssets["LimePlayer"] = {32, 0, 8, 8, 4, 4};
  TextureAssets["OrangePlayer"] = {40, 0, 8, 8, 4, 4};


  // Bullets
  TextureAssets["PinkBullet"] = {0, 8, 4, 4, 2, 2};
  TextureAssets["GreenBullet"] = {4, 8, 4, 4, 2, 2};
  TextureAssets["RedBullet"] = {8, 8, 4, 4, 2, 2};
  TextureAssets["CyanBullet"] = {12, 8, 4, 4, 2, 2};
  TextureAssets["LimeBullet"] = {16, 8, 4, 4, 2, 2};
  TextureAssets["OrangeBullet"] = {20, 8, 4, 4, 2, 2};

  // Fill background
  for(int i = 0; i <= 14; i++)
  {
    for(int j = 0; j <= 10; j++)
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

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) SendShooting(3.32f);

    // Debug
    //if (IsKeyPressed(KEY_KP_ADD)) MainCamera.zoom += 1.0f;
    //if (IsKeyPressed(KEY_KP_SUBTRACT)) MainCamera.zoom -= 1.0f;

    // Update

    // Render
    BeginDrawing();
    ClearBackground({167, 167, 167, 255});
    
    BeginMode2D(MainCamera);

    BackgroundRendererSystem(Scene, TextureAtlas, TextureAssets);
    ShadowRendererSystem(Scene, TextureAtlas, TextureAssets);
    SpriteRendererSystem(Scene, TextureAtlas, TextureAssets);
    ColliderDebugRendererSystem(Scene);
    //HealthRendererSystem(Scene);

    EndMode2D();

    DrawFPS(8, 4);
    DrawText(IsConnected() ? "Connected" : "Disconnected", 8, 24, 24,
     IsConnected() ? GREEN : RED);

    EndDrawing();
  }

  Disconnect();
  DeInitENet();
  CloseWindow();
}