#pragma once

#include "enet/enet.h"
#include "entt/entity/registry.hpp"

#include "Components.hpp"
#include "NetworkTypes.hpp"
#include "SvNetworking.hpp"

void NetworkPlayerUpdateSystem(entt::registry& Scene, ENetHost* Server);