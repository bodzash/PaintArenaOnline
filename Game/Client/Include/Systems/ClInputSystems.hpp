#pragma once

#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Components.hpp"
#include "Networking.hpp"
#include "Math.hpp"

void PlayerNetworkInputSystem(entt::registry& Scene);