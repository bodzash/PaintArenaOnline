#pragma once

#include <array>
#include <string>

/*
struct SpriteAsset
{
  float x = 0.0f;
  float y = 0.0f;
  float Width = 8.0f;
  float Height = 8.0f;
  float OffsetX = 0.0f;
  float OffsetY = 0.0f;
};
*/

std::array<std::string, 6> NetworkIdToPlayerAsset = {"PinkPlayer", "GreenPlayer",
  "RedPlayer", "CyanPlayer", "LimePlayer", "OrangePlayer"};

std::array<std::string, 6> NetworkIdToBulletAsset = {"PinkBullet", "GreenBullet",
  "RedBullet", "CyanBullet", "LimeBullet", "OrangeBullet"};