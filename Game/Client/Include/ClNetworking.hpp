#pragma once

#include "entt/entity/registry.hpp"

bool IsConnected();
void ConnectToServer();
void PollNetwork(entt::registry& Scene);
void SendMovement(bool bLeft, bool bRight, bool bUp, bool bDown);
void SendShooting(float Angle);
uint8_t GetSelfNetworkId();
entt::entity GetSelfPlayerEntity();
void Disconnect();
void DeInitENet();