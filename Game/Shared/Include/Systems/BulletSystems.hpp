#pragma once

#include "entt/entity/registry.hpp"

#include "Components.hpp"
#include "Math.hpp"

void ClientBulletDamageSystem(entt::registry& Scene);
void ServerBulletDamageSystem(entt::registry& Scene);