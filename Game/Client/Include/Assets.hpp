#pragma once

#include <string>
#include <map>

#include "raylib.h"

#include "Core.hpp"

using std::string;

void LoadAllTextureAssets(std::map<string, SpriteAsset>& TextureAssets);
void LoadAllAudioAssets(std::map<string, Sound>& AudioAssets);
void LoadAllFontAssets(std::map<string, Font>& FontAssets);