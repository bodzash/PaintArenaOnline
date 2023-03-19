#pragma once

#include "entt/entity/registry.hpp"
#include "enet/enet.h"

#include "Components.hpp"
#include "Math.hpp"
#include "NetworkTypes.hpp"
#include "SvNetworking.hpp"

void HealthDeathSystem(entt::registry& Scene, ENetHost* Server);