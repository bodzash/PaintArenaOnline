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
#include "UIComponents.hpp"
#include "Systems/AudioSystems.hpp"
#include "Systems/BulletSystems.hpp"
#include "Systems/RendererSystems.hpp"
#include "Systems/ShakeEffectSystems.hpp"
#include "Systems/SmudgeEffectSystems.hpp"
#include "Systems/ClInputSystems.hpp"
#include "Systems/MovementSystems.hpp"
#include "Systems/BoundsSystems.hpp"
#include "Systems/UISystems.hpp"

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
  if (!InitClientPeer()) return EXIT_FAILURE;

  // Setup
  entt::registry Scene;
  Texture2D TextureAtlas = LoadTexture("./Resources/SpriteAtlas.png");
  std::map<string, SpriteAsset> TextureAssets;
  std::map<string, Sound> AudioAssets;
  std::map<string, Font> FontAssets;
  Camera2D MainCamera;
  bool bMuted = false;
  MainCamera.target = {0.0f, 0.0f};
  MainCamera.offset = {0.0f, 0.0f};
  MainCamera.rotation = 0.0f;
  MainCamera.zoom = 4.0f;

  // Load Assets
  LoadAllTextureAssets(TextureAssets);
  LoadAllAudioAssets(AudioAssets);
  LoadAllFontAssets(FontAssets);

  // Create background
  CreatePrefabTiles(Scene);

  // Menu test TODO tidy up
  entt::entity Label = Scene.create();
  Scene.emplace<UILabel>(Label, (Vector2){200.0f, 200.0f}, "This is a label :D",
    GREEN, 32.0f);

  entt::entity Mute = Scene.create();
  Scene.emplace<UILabel>(Mute, (Vector2){4.0f, (704.f - (32.0f + 2.0f))},
    "Press M to toggle Mute", BLACK, 32.0f);

  entt::entity Input = Scene.create();
  UILabel Lab = {(Vector2){8.0f, 2.0f}, "Default", BLACK, 32.0f};
  Scene.emplace<UIInputBox>(Input, (Rectangle){60.0f, 60.0f, 240.0f, 40.0f}, 16, Lab);

  // Connect to server TODO move this and do checking
  ConnectToServer("127.0.0.1", 7777);
  
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
    ShadowFollowOwnerSystem(Scene);

    // Render
    BeginDrawing();
      // Clear canvas
      ClearBackground({167, 167, 167, 255});
      // Render 2D
      BeginMode2D(MainCamera);
      SpriteRendererSystem(Scene, TextureAtlas, TextureAssets);
      // TODO move this and rework it as well
      CursorRenderingSystem(TextureAtlas, TextureAssets);
      EndMode2D();
      // Render UI
      UIInputBoxSystem(Scene, FontAssets);
      UIRenderLabelSystem(Scene, FontAssets);
      // Debug
      DrawFPS(8, 4);
      DrawText(IsConnected() ? "Connected" : "Disconnected", 8, 24, 24,
        IsConnected() ? GREEN : RED);
    EndDrawing();
  }

  // Cleanup
  Disconnect();
  DeInitENet();
  CloseAudioDevice();
  CloseWindow();
}