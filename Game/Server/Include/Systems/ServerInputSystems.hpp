#pragma once

#include <cmath>

#include "entt/entity/registry.hpp"

#include "Components.hpp"
#include "NetworkTypes.hpp"

void ResetPlayerInputSystem(entt::registry& Scene);

void ApplyNetworkInputToPlayer(entt::registry& Scene, entt::entity Player,
  ClientMovementCommands* Cmd);
  
void InputToMovementSystem(entt::registry& Scene);