#include "ClPrefabs.hpp"

entt::entity CreatePrefabSmudgeSmall(entt::registry& Scene, float x, float y,
  uint8_t NetId)
{
  int Index = RandomInt(3);

  string Asset = "SmallSmudge1";
  if (Index == 1) Asset = "SmallSmudge2";
  else if (Index == 2) Asset = "SmallSmudge3";

  entt::entity Effect = Scene.create();
  Scene.emplace<Position>(Effect, x, y);
  Scene.emplace<Sprite>(Effect, Asset, 1, (int)NetId);
  Scene.emplace<Audio>(Effect, "Splash1", "Splash2", true, true);

  return Effect;
}

// Thanks raylib team for removing DrawTextureTiled <3
void CreatePrefabTiles(entt::registry& Scene)
{
  for(int i = 0; i <= 14; i++)
  {
    for(int j = 0; j <= 10; j++)
    {
      entt::entity Tile = Scene.create();
      Scene.emplace<Position>(Tile, 16.0f * i, 16.0f * j);
      Scene.emplace<Sprite>(Tile, "FloorTile", 0, 6);
    }
  }
}

entt::entity CreatePrefabPlayer(entt::registry& Scene, uint8_t Team, float x, float y,
  string Skin)
{
  entt::entity Shdw = Scene.create();
  Scene.emplace<Position>(Shdw, x, y);
  Scene.emplace<Sprite>(Shdw, "PlayerShadow", 2, 7, 4.0f, -2.0f);

  entt::entity Player = Scene.create();
  Scene.emplace<PlayerTag>(Player);
  Scene.emplace<TeamId>(Player,Team);
  Scene.emplace<NetworkId>(Player,Team);
  Scene.emplace<Position>(Player, x, y);
  Scene.emplace<Collider>(Player, 8.0f);
  Scene.emplace<Audio>(Player, "Hit1", "Hit2", false, false);
  Scene.emplace<Sprite>(Player, Skin, 3, 6, 4.0f, 4.0f, 4.0f, 4.0f);
  Scene.emplace<Shadow>(Player, (uint32_t)Shdw);
  Scene.emplace<Shake>(Player);

  return Player;
}

void CreatePrefabBullet(entt::registry& Scene, uint8_t Team, float x, float y, float Dir,
  string Skin)
{
  entt::entity Shdw = Scene.create();
  Scene.emplace<Position>(Shdw, x, y);
  Scene.emplace<Sprite>(Shdw, "BulletShadow", 2, 7, 2.0f, -3.0f);

  entt::entity Bullet = Scene.create();
  Scene.emplace<BulletTag>(Bullet);
  Scene.emplace<TeamId>(Bullet, Team);
  Scene.emplace<Position>(Bullet, x, y);
  Scene.emplace<Direction>(Bullet, Dir);
  Scene.emplace<Speed>(Bullet, 140.0f);
  Scene.emplace<Collider>(Bullet, 4.0f);
  Scene.emplace<Audio>(Bullet, "Shoot1", "Shoot2", true, true);
  Scene.emplace<Sprite>(Bullet, Skin, 4, 6, 2.0f, 2.0f);
  Scene.emplace<Shadow>(Bullet, (uint32_t)Shdw);
}

void CreatePrefabBigSmudge(entt::registry& Scene, uint8_t Team, float x, float y)
{
  int Index = RandomInt(3);
  string Asset = "BigSmudge1";
  if (Index == 1) Asset = "BigSmudge2";
  else if (Index == 2) Asset = "BigSmudge3";

  entt::entity Effect = Scene.create();
  Scene.emplace<Position>(Effect, x, y);
  Scene.emplace<Sprite>(Effect, Asset, 1, (int)Team);
}

void CreatePrefabSmudgeBall(entt::registry& Scene, uint8_t Team, string Skin,
  float x, float y)
{
  entt::entity Shdw = Scene.create();
  Scene.emplace<Position>(Shdw, x, y);
  Scene.emplace<Sprite>(Shdw, "BulletShadow", 2, 7, 2.0f, -3.0f);

  entt::entity Ball = Scene.create();
  Scene.emplace<Position>(Ball, x, y);
  Scene.emplace<Direction>(Ball, (float)(rand() % 628) / 100.0f);
  Scene.emplace<Speed>(Ball, (float)((rand() % 130 - 80) + 80));
  Scene.emplace<TeamId>(Ball, Team);
  Scene.emplace<Sprite>(Ball, Skin, 1, 6, 2.0f, 2.0f);
  Scene.emplace<SmudgeBall>(Ball, 5.0f + (float)(rand() % 10));
  Scene.emplace<Shadow>(Ball, (uint32_t)Shdw);
}