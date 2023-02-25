#pragma once

#include "entt/entity/registry.hpp"

void ConnectToServer();
void PollNet(entt::registry& Scene);
void SendPacket();
void SendMovement(bool Left, bool Right, bool Up, bool Down);
void Disconnect();