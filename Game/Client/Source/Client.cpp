#include <iostream>
#include <string>
#include <map>
#include <thread>

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
  //bool bGameShouldRun = true;
  MainCamera.target = {0.0f, 0.0f};
  MainCamera.offset = {0.0f, 0.0f};
  MainCamera.rotation = 0.0f;
  MainCamera.zoom = 4.0f;

  // Load Assets
  LoadAllTextureAssets(TextureAssets);
  LoadAllAudioAssets(AudioAssets);
  LoadAllFontAssets(FontAssets);

  // Place a toggle mute label
  entt::entity Mute = Scene.create();
  Scene.emplace<UILabel>(Mute, (Vector2){4.0f, (704.f - (32.0f + 2.0f))},
    "Press M to toggle Mute", 32.0f);

  // Create background
  CreatePrefabTiles(Scene);

  // Menu test TODO tidy
  entt::entity Input = Scene.create();
  Rectangle InpBox = {480.0f - 158.0f, 200.0f, 316.0f, 40.0f};
  UILabel Lab = {(Vector2){8.0f, 2.0f}, "", 32.0f};
  Scene.emplace<UIInputBox>(Input, InpBox, 15, Lab, "Enter IP here");

  entt::entity StatusLabel = Scene.create();
  Scene.emplace<UILabel>(StatusLabel, (Vector2){480.0f - 158.0f, 288.0f}, "", 24.0f);

  entt::entity Btn = Scene.create();
  Rectangle BtnBox = {480.0f - 158.0f, 248.0f, 316.0f, 40.0f};
  UILabel BtnLab = {(Vector2){32.0f, 2.0f}, "Connect to Server", 32.0f};
  Scene.emplace<UIButton>(Btn, BtnBox, BtnLab, [&Scene, &Input, &StatusLabel, &Btn](){
    auto& Inp = Scene.get<UIInputBox>(Input);
    auto& Status = Scene.get<UILabel>(StatusLabel);

    Status.Text = "Connecting to server...";

    if (ConnectToServer(Inp.Label.Text, 7777))
    {
      Scene.destroy(Input);
      Scene.destroy(StatusLabel);

      Scene.destroy(Btn);
    }
    else
    {
      Status.Text = "Connection failed... Try again!";
    }
  });
  
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
      UIRenderButtonSystem(Scene, FontAssets);
      // Debug
      DrawFPS(8, 4);
      DrawText(IsConnected() ? "Connected" : "Disconnected", 8, 24, 24,
        IsConnected() ? GREEN : RED);
    EndDrawing();
  }

  // Cleanup
  DeInitENet();
  CloseAudioDevice();
  CloseWindow();
}