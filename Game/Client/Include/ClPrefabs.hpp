#pragma once

#include <string>
#include <cstdint>

#include "entt/entity/registry.hpp"

#include "Math.hpp"
#include "Components.hpp"

using std::string;

entt::entity CreatePrefabSmudgeSmall(entt::registry& Scene, float x, float y,
  uint8_t NetId);
void CreatePrefabTiles(entt::registry& Scene);