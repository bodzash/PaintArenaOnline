#pragma once

#include <string>

#include "entt/entity/registry.hpp"

#include "Math.hpp"
#include "Components.hpp"

using std::string;

entt::entity CreatePrefabSmudgeSmall(entt::registry& Scene, float x, float y,
  uint8_t NetId);
entt::entity CreatePrefabPlayer(entt::registry& Scene, uint8_t Team, float x, float y,
  string Skin);
void CreatePrefabTiles(entt::registry& Scene);
void CreatePrefabBullet(entt::registry& Scene, uint8_t Team, float x, float y, float Dir,
  string Skin);
void CreatePrefabBigSmudge(entt::registry& Scene, uint8_t Team, float x, float y);
void CreatePrefabSmudgeBall(entt::registry& Scene, uint8_t Team, string Skin,
  float x, float y);