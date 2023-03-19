#pragma once

#include <iostream>
#include <array>
#include <cstdint>

#include "enet/enet.h"
#include "entt/entity/registry.hpp"

#include "NetworkTypes.hpp"
#include "SvPrefabs.hpp"
#include "Systems/SvInputSystems.hpp"

template<typename T>
void ServerSendMessage(T& Msg, ENetPeer* Peer)
{
  ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
  enet_peer_send(Peer, 0, Packet);
}

template<typename T>
void ServerBroadcastMessage(T& Msg, ENetHost* Server)
{
  ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
  enet_host_broadcast(Server, 0, Packet);
}

void HandleClientConnect(ENetEvent& Event, entt::registry& Scene, ENetHost* pServer,
  std::array<RemotePeer, 6>& NetworkClients);
void HandleClientDisconnect(ENetEvent& Event, entt::registry& Scene, ENetHost* pServer);
void HandleServerReceive(ENetEvent& Event, entt::registry& Scene, ENetHost* pServer);