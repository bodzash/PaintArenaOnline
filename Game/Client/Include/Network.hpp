#pragma once

#include "entt/entity/registry.hpp"

void ConnectToServer();
void PollNetwork(entt::registry& Scene);
void SendPacket();
void SendMovement(bool Left, bool Right, bool Up, bool Down);
void Disconnect();