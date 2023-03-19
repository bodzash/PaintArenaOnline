#pragma once

#include <cstdint>

#include "entt/entity/registry.hpp"

#include "Components.hpp"
#include "Math.hpp"

entt::entity CreatePrefabPlayer(entt::registry& Scene, uint8_t NetId);
entt::entity CreatePrefabBullet(entt::registry& Scene, float x, float y, float Angle,
  uint8_t NetId);