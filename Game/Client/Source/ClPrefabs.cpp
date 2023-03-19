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