#pragma once

#include "entt/entity/registry.hpp"

bool IsConnected();
void ConnectToServer();
void PollNetwork(entt::registry& Scene);
void SendPacket();
void SendMovement(bool bLeft, bool bRight, bool bUp, bool bDown);
void Disconnect();
void DeInitENet();