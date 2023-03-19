#include <iostream>
#include <string>
#include <map>

#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Core.hpp"
#include "Assets.hpp"
#include "ClNetworking.hpp"
#include "ClPrefabs.hpp"
#include "Components.hpp"
#include "Systems/AudioSystems.hpp"
#include "Systems/BulletSystems.hpp"
#include "Systems/RendererSystems.hpp"
#include "Systems/ShakeEffectSystems.hpp"
#include "Systems/SmudgeEffectSystems.hpp"
#include "Systems/ClInputSystems.hpp"
#include "Systems/MovementSystems.hpp"
#include "Systems/BoundsSystems.hpp"

using std::string;

int main(void)
{
  const int ScreenWidth = 224;
  const int ScreenHeight = 160;
  const int WindowWidth = 960;
  const int WindowHeight = 704;

  // Init
  InitWindow(WindowWidth, WindowHeight, "Paint Arena Online");
  InitAudioDevice();
  SetWindowIcon(LoadImage("./Resources/WindowIcon.png"));
  SetTargetFPS(60);
  HideCursor();
  SetRandomSeed();

  // Setup
  entt::registry Scene;
  Texture2D TextureAtlas = LoadTexture("./Resources/SpriteAtlas.png");
  std::map<string, SpriteAsset> TextureAssets;
  std::map<string, Sound> AudioAssets;
  Camera2D MainCamera;
  bool bMuted = false;
  MainCamera.target = {0.0f, 0.0f};
  MainCamera.offset = {0.0f, 0.0f};
  MainCamera.rotation = 0.0f;
  MainCamera.zoom = 4.0f;

  // Load Assets
  LoadAllTextureAssets(TextureAssets);
  LoadAllAudioAssets(AudioAssets);

  // Fill background
  CreatePrefabTiles(Scene);

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
    if (IsKeyPressed(KEY_M))
    {
      if (bMuted)SetMasterVolume(1.0f);
      else SetMasterVolume(0.0f);
      bMuted = !bMuted;
    }

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