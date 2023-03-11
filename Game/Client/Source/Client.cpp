#include <iostream>
#include <string>
#include <map>

#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Network.hpp"
#include "Components.hpp"
#include "BulletSystem.hpp"

#define PlayerColorPink (Color){133, 45, 102, 255}
#define PlayerColorGreen (Color){48, 156, 99, 255}
#define PlayerColorRed (Color){201, 48, 56, 255}
#define PlayerColorCyan (Color){99, 194, 201, 255}
#define PlayerColorLime (Color){180, 214, 69, 255}
#define PlayerColorOrange (Color){255, 173, 59, 255}

using std::string;

Color IndexToRGB[6] = {PlayerColorPink, PlayerColorGreen, PlayerColorRed,
  PlayerColorCyan, PlayerColorLime, PlayerColorOrange};

struct SpriteAsset
{
  float x = 0.0f;
  float y = 0.0f;
  float Width = 8.0f;
  float Height = 8.0f;
  float OffsetX = 0.0f;
  float OffsetY = 0.0f;
};

void CreatePrefabSmudgeSmall(entt::registry& Scene, float x, float y, uint8_t NetId)
{
  int Index = RandomInt(3);

  string Asset = "SmallSmudge1";
  if (Index == 1) Asset = "SmallSmudge2";
  else if (Index == 2) Asset = "SmallSmudge3";

  entt::entity Effect = Scene.create();
  Scene.emplace<Position>(Effect, x, y);
  Scene.emplace<Smudge>(Effect, Asset, (int)NetId);
  Scene.emplace<Audio>(Effect, (Index == 1) ? "Splash1" : "Splash2", true);
}

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
      {Pos.x - Spr.OffsetX, Pos.y - Spr.OffsetY},
      WHITE);
  }
}

void SmudgeRendererSystem(entt::registry& Scene, Texture2D Tex,
  std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<Position, Smudge>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Spr = View.get<Smudge>(Entity);

    DrawTextureRec(Tex, {Assets[Spr.Asset].x, Assets[Spr.Asset].y,
      Assets[Spr.Asset].Width, Assets[Spr.Asset].Height},
      {Pos.x - Assets[Spr.Asset].OffsetX, Pos.y - Assets[Spr.Asset].OffsetY},
      IndexToRGB[Spr.Color]);
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

void HealthDebugRendererSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position, Health>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Hel = View.get<Health>(Entity);

    DrawText(std::to_string(Hel.Current).c_str(), Pos.x + 12, Pos.y, 8, BLACK);
    //DrawText(std::to_string(Pos.y).c_str(), Pos.x + 12, Pos.y + 10, 8, BLACK);
  }
}

void SmudgeBallSystem(entt::registry& Scene, std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<Position, SmudgeBall, Sprite, TeamId, Speed>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Smb = View.get<SmudgeBall>(Entity);
    auto& Spr = View.get<Sprite>(Entity);
    auto& Spd = View.get<Speed>(Entity);
    auto& Tid = View.get<TeamId>(Entity);

    Spr.OffsetY = Smb.Height;

    Spd.MaxSpeed *= 0.9f;
    Smb.Height -= 0.7f;

    if (Smb.Height < -1.0f)
    {
      CreatePrefabSmudgeSmall(Scene, Pos.x, Pos.y, Tid.Team);
      Scene.destroy(Entity);
    }
  }
}

void AudioPlayerSystem(entt::registry& Scene, std::map<string, Sound> Assets)
{
  auto View = Scene.view<Audio>();
  for (auto Entity : View)
  {
    auto& Aud = View.get<Audio>(Entity);

    PlaySound(Assets[Aud.Asset]);
    Scene.remove<Audio>(Entity);
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
  InitAudioDevice();
  SetTargetFPS(60);
  HideCursor();

  entt::registry Scene;
  Texture2D TextureAtlas = LoadTexture("./Resources/SpriteAtlas.png");
  std::map<string, SpriteAsset> TextureAssets;
  std::map<string, Sound> AudioAssets;
  Camera2D MainCamera;
  MainCamera.target = {0.0f, 0.0f};
  MainCamera.offset = {0.0f, 0.0f};
  MainCamera.rotation = 0.0f;
  MainCamera.zoom = 4.0f;

  // TODO cleanup
  // Misc
  TextureAssets["FloorTile"] = {0, 12, 16, 16};
  TextureAssets["Cursor"] = {16, 12, 10, 10, 5, 5};
  TextureAssets["PlayerShadow"] = {24, 8, 8, 3, 4, -2};
  TextureAssets["BulletShadow"] = {32, 8, 4, 3, 2, -3};

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

  // Small Smudges
  TextureAssets["SmallSmudge1"] = {26, 11, 8, 8, 4, 4};
  TextureAssets["SmallSmudge2"] = {34, 11, 8, 8, 4, 4};
  TextureAssets["SmallSmudge3"] = {42, 11, 8, 8, 4, 4};

  // Big Smudges
  TextureAssets["BigSmudge1"] = {0, 28, 16, 16, 8, 8};
  TextureAssets["BigSmudge2"] = {16, 28, 16, 16, 8, 8};
  TextureAssets["BigSmudge3"] = {32, 28, 16, 16, 8, 8};

  // Shoot
  AudioAssets["Shoot1"] = LoadSound("./Resources/Shoot1.ogg");
  AudioAssets["Shoot2"] = LoadSound("./Resources/Shoot2.ogg");

  // Hit
  //AudioAssets["Hit1"] = LoadSound("./Resources/Hit1.ogg");
  //AudioAssets["Hit2"] = LoadSound("./Resources/Hit2.ogg");

  // Splash
  AudioAssets["Splash1"] = LoadSound("./Resources/Splash1.ogg");
  AudioAssets["Splash2"] = LoadSound("./Resources/Splash2.ogg");

  // Fill background
  for(int i = 0; i <= 14; i++)
  {
    for(int j = 0; j <= 10; j++)
    {
      entt::entity Tile = Scene.create();
      Scene.emplace<TileTag>(Tile);
      Scene.emplace<Position>(Tile, 16.0f * i, 16.0f * j);
    }
  }

  ConnectToServer();
  
  while (!WindowShouldClose())
  {
    // Network stuff
    PollNetwork(Scene);

    // TODO clean up
    if (IsConnected())
    {
      entt::entity MyPlayerId = GetSelfPlayerEntity();
      uint8_t MyNetworkId = GetSelfNetworkId();
      bool bLeft = IsKeyDown(KEY_A) ;
      bool bRight = IsKeyDown(KEY_D);
      bool bUp = IsKeyDown(KEY_W);
      bool bDown = IsKeyDown(KEY_S);

      // Calculate angle
      if (MyNetworkId != 255)
      {
        auto& Pos = Scene.get<Position>(MyPlayerId);
        float Angle = PointDirection(Pos.x, Pos.y, GetMouseX() / 4, GetMouseY() / 4);
        
        if (bLeft || bRight || bUp || bDown) SendMovement(bLeft, bRight, bUp, bDown);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) SendShooting(Angle);

        //if (IsKeyPressed(KEY_F)) PlaySound(AudioAssets["Shoot1"]);
        //if (IsKeyPressed(KEY_E)) PlaySound(AudioAssets["Splash1"]);
      }
    }
    
    // Debug
    //if (IsKeyPressed(KEY_KP_ADD)) MainCamera.zoom += 1.0f;
    //if (IsKeyPressed(KEY_KP_SUBTRACT)) MainCamera.zoom -= 1.0f;

    // Update
    BulletMovementSystem(Scene, GetFrameTime());
    BulletDamageSystem(Scene, false);
    RemoveBulletOutOfBoundsSystem(Scene);
    SmudgeBallSystem(Scene, TextureAssets);
    AudioPlayerSystem(Scene, AudioAssets);

    // Render
    BeginDrawing();
    ClearBackground({167, 167, 167, 255});
    
    BeginMode2D(MainCamera);

    BackgroundRendererSystem(Scene, TextureAtlas, TextureAssets);
    SmudgeRendererSystem(Scene, TextureAtlas, TextureAssets);
    ShadowRendererSystem(Scene, TextureAtlas, TextureAssets);
    SpriteRendererSystem(Scene, TextureAtlas, TextureAssets);
    //HealthDebugRendererSystem(Scene);
    //ColliderDebugRendererSystem(Scene);

    // Render cursor TODO cleanup
    DrawTextureRec(TextureAtlas, {TextureAssets["Cursor"].x, TextureAssets["Cursor"].y,
      TextureAssets["Cursor"].Width, TextureAssets["Cursor"].Height},
      {(float)GetMouseX() / 4 - 5, (float)GetMouseY() / 4 - 5}, WHITE);

    EndMode2D();

    DrawFPS(8, 4);
    DrawText(IsConnected() ? "Connected" : "Disconnected", 8, 24, 24,
     IsConnected() ? GREEN : RED);
    /*
    DrawText(std::to_string(GetMouseX() / 4).c_str(), 8, 32, 24, BLACK);
    DrawText(std::to_string(GetMouseY() / 4).c_str(), 8, 64, 24, BLACK);
    */

    EndDrawing();
  }

  Disconnect();
  DeInitENet();
  CloseAudioDevice();
  CloseWindow();
}