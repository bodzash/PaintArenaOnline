#pragma once

#include "entt/entity/registry.hpp"

#include "Components.hpp"
#include "Math.hpp"

// TODO move this function to some physicsdynamicssystems.hpp/cpp
void DirectionalMovementSystem(entt::registry& Scene, float Delta);

void RemoveBulletOutOfBoundsSystem(entt::registry& Scene);

// TODO move this function to server NOt shared
void ServerBulletDamageSystem(entt::registry& Scene);