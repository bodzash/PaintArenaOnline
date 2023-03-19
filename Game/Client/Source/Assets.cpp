#include "Assets.hpp"

void LoadAllTextureAssets(std::map<string, SpriteAsset>& TextureAssets)
{
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
}

void LoadAllAudioAssets(std::map<string, Sound>& AudioAssets)
{
    // Shoot
  AudioAssets["Shoot1"] = LoadSound("./Resources/Shoot1.ogg");
  AudioAssets["Shoot2"] = LoadSound("./Resources/Shoot2.ogg");

  // Hit
  AudioAssets["Hit1"] = LoadSound("./Resources/Hit1.ogg");
  AudioAssets["Hit2"] = LoadSound("./Resources/Hit2.ogg");

  // Splash
  AudioAssets["Splash1"] = LoadSound("./Resources/Splash1.ogg");
  AudioAssets["Splash2"] = LoadSound("./Resources/Splash2.ogg");
}