#pragma once

#include <cstdint>
#include <string>
#include <map>

#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Core.hpp"
#include "Components.hpp"

using std::string;

#define PlayerColorPink (Color){133, 45, 102, 255}
#define PlayerColorGreen (Color){48, 156, 99, 255}
#define PlayerColorRed (Color){201, 48, 56, 255}
#define PlayerColorCyan (Color){99, 194, 201, 255}
#define PlayerColorLime (Color){180, 214, 69, 255}
#define PlayerColorOrange (Color){255, 173, 59, 255}

void SpriteRendererSystem(entt::registry& Scene, Texture2D& Tex,
  std::map<string, SpriteAsset>& Assets);
void CursorRenderingSystem(Texture2D& Tex, std::map<string, SpriteAsset>& Assets);