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
  Scene.emplace<Smudge>(Effect, Asset, (int)NetId);
  Scene.emplace<Audio>(Effect, "Splash1", "Splash2", true, true);

  return Effect;
}

void CreatePrefabTiles(entt::registry& Scene)
{
  for(int i = 0; i <= 14; i++)
  {
    for(int j = 0; j <= 10; j++)
    {
      entt::entity Tile = Scene.create();
      Scene.emplace<TileTag>(Tile);
      Scene.emplace<Position>(Tile, 16.0f * i, 16.0f * j);
    }
  }
}

entt::entity CreatePrefabPlayer(entt::registry& Scene, uint8_t Team, float x, float y,
  string Skin)
{
  entt::entity Player = Scene.create();
  Scene.emplace<PlayerTag>(Player);
  Scene.emplace<TeamId>(Player,Team);
  Scene.emplace<NetworkId>(Player,Team);
  Scene.emplace<Position>(Player, x, y);
  Scene.emplace<Collider>(Player, 8.0f);
  Scene.emplace<Audio>(Player, "Hit1", "Hit2", false, false);
  Scene.emplace<Sprite>(Player, Skin, 4.0f, 4.0f, 4.0f, 4.0f);
  Scene.emplace<Shadow>(Player, "PlayerShadow");
  Scene.emplace<Shake>(Player);

  return Player;
}

void CreatePrefabBullet(entt::registry& Scene, uint8_t Team, float x, float y, float Dir,
  string Skin)
{
  entt::entity Bullet = Scene.create();
  Scene.emplace<BulletTag>(Bullet);
  Scene.emplace<TeamId>(Bullet, Team);
  Scene.emplace<Position>(Bullet, x, y);
  Scene.emplace<Direction>(Bullet, Dir);
  Scene.emplace<Speed>(Bullet, 140.0f);
  Scene.emplace<Collider>(Bullet, 4.0f);
  Scene.emplace<Audio>(Bullet, "Shoot1", "Shoot2", true, true);
  Scene.emplace<Sprite>(Bullet, Skin, 2.0f, 2.0f);
  Scene.emplace<Shadow>(Bullet, "BulletShadow");
}

void CreatePrefabBigSmudge(entt::registry& Scene, uint8_t Team, float x, float y)
{
  int Index = RandomInt(3);
  string Asset = "BigSmudge1";
  if (Index == 1) Asset = "BigSmudge2";
  else if (Index == 2) Asset = "BigSmudge3";

  entt::entity Effect = Scene.create();
  Scene.emplace<Position>(Effect, x, y);
  Scene.emplace<Smudge>(Effect, Asset, (int)Team);
}

void CreatePrefabSmallSmudge(entt::registry& Scene, uint8_t Team, string Skin,
  float x, float y)
{
  entt::entity Ball = Scene.create();
  Scene.emplace<Position>(Ball, x, y);
  Scene.emplace<Direction>(Ball, (float)(rand() % 628) / 100.0f);
  Scene.emplace<Speed>(Ball, (float)((rand() % 130 - 80) + 80));
  Scene.emplace<TeamId>(Ball, Team);
  Scene.emplace<Sprite>(Ball, Skin, 2.0f, 2.0f);
  Scene.emplace<Shadow>(Ball, "BulletShadow");
  Scene.emplace<SmudgeBall>(Ball, 5.0f + (float)(rand() % 10));
}