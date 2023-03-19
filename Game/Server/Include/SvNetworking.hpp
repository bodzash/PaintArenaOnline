#pragma once

#include "enet/enet.h"

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