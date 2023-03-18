#pragma once

#include "entt/entity/registry.hpp"

#include "Components.hpp"

void RemoveBulletOutOfBoundsSystem(entt::registry& Scene);
void KeepPlayerInBoundsSystem(entt::registry& Scene);