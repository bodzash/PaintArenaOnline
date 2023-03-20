#pragma once

#include "entt/entity/registry.hpp"

#include "Math.hpp"
#include "Components.hpp"

void DynamicMovementSystem(float DeltaTime, entt::registry& Scene);
void DirectionalMovementSystem(entt::registry& Scene, float Delta);
void ShadowFollowOwnerSystem(entt::registry& Scene);