#include <iostream>
#include <string>
#include <map>

#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Core.hpp"
#include "Network.hpp"
#include "Components.hpp"
#include "Systems/AudioSystems.hpp"
#include "Systems/BulletSystems.hpp"
#include "Systems/RendererSystems.hpp"
#include "Systems/ShakeEffectSystems.hpp"
#include "Systems/SmudgeEffectSystems.hpp"
#include "Systems/ClientInputSystems.hpp"

using std::string;

void CreatePrefabSmudgeSmall(entt::registry& Scene, float x, float y, uint8_t NetId)
{
  int Index = RandomInt(3);

  string Asset = "SmallSmudge1";
  if (Index == 1) Asset = "SmallSmudge2";
  else if (Index == 2) Asset = "SmallSmudge3";

  entt::entity Effect = Scene.create();
  Scene.emplace<Position>(Effect, x, y);
  Scene.emplace<Smudge>(Effect, Asset, (int)NetId);
  Scene.emplace<Audio>(Effect, "Splash1", "Splash2", true, true);
}

int main(void)
{
  // Init
  const int ScreenWidth = 224;
  const int ScreenHeight = 160;
  const int WindowWidth = 960;
  const int WindowHeight = 704;

  InitWindow(WindowWidth, WindowHeight, "Paint Arena Online");
  InitAudioDevice();
  SetWindowIcon(LoadImage("./Resources/WindowIcon.png"));
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
  AudioAssets["Hit1"] = LoadSound("./Resources/Hit1.ogg");
  AudioAssets["Hit2"] = LoadSound("./Resources/Hit2.ogg");

  // Splash
  AudioAssets["Splash1"] = LoadSound("./Resources/Splash1.ogg");
  AudioAssets["Splash2"] = LoadSound("./Resources/Splash2.ogg");

  // Fill background
  for(int i = 0; i <= 14; i++)
  {
    for(int j = 0; j <= 10; j++)
    {
      // Craete Prefab Tile
      entt::entity Tile = Scene.create();
      Scene.emplace<TileTag>(Tile);
      Scene.emplace<Position>(Tile, 16.0f * i, 16.0f * j);
    }
  }

  // Connect to server
  ConnectToServer();
  
  // Game loop
  while (!WindowShouldClose())
  {
    // Network
    PollNetwork(Scene);

    // Input
    PlayerNetworkInputSystem(Scene);

    // Debug
    if (IsKeyPressed(KEY_M)) SetMasterVolume(0.0f);
    if (IsKeyPressed(KEY_N)) SetMasterVolume(1.0f);

    // Update
    AudioPlayerSystem(Scene, AudioAssets);
    DirectionalMovementSystem(Scene, GetFrameTime());
    ClientBulletDamageSystem(Scene);
    RemoveBulletOutOfBoundsSystem(Scene);
    SmudgeBallSystem(Scene, TextureAssets);
    ShakeSpriteSystem(Scene);

    // Render
    BeginDrawing();
      ClearBackground({167, 167, 167, 255});
      BeginMode2D(MainCamera);
        BackgroundRendererSystem(Scene, TextureAtlas, TextureAssets);
        SmudgeRendererSystem(Scene, TextureAtlas, TextureAssets);
        ShadowRendererSystem(Scene, TextureAtlas, TextureAssets);
        SpriteRendererSystem(Scene, TextureAtlas, TextureAssets);
        //ColliderDebugRendererSystem(Scene);
        CursorRenderingSystem(TextureAtlas, TextureAssets);
      EndMode2D();
      // Debug START
      DrawFPS(8, 4);
      DrawText(IsConnected() ? "Connected" : "Disconnected", 8, 24, 24,
        IsConnected() ? GREEN : RED);
      // Debug END
    EndDrawing();
  }

  // Cleanup
  Disconnect();
  DeInitENet();
  CloseAudioDevice();
  CloseWindow();
}