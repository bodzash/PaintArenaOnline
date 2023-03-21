#include <iostream>
#include <string>
#include <cmath>

#include "enet/enet.h"

#include "ClNetworking.hpp"
#include "Components.hpp"
#include "NetworkTypes.hpp"
#include "ClientTypes.hpp"
#include "ClPrefabs.hpp"

using std::string;

ENetAddress Address;
ENetEvent Event;
ENetPeer* Peer;
ENetHost* Client;

std::array<RemotePeer, 6> NetworkClients;

bool bConnected = false;
uint8_t SelfNetworkId = 255;

bool IsConnected()
{
  return bConnected;
}

bool InitClientPeer()
{
  try
  {
    // Init
    if (enet_initialize() != 0) throw 0;

    // Create client
    Client = enet_host_create(NULL, 1, 1, 0, 0);
    if (Client == NULL) throw 1;

    return true;
  }
  catch(const int ErrorCode)
  {
    return false;
  }
}

bool ConnectToServer(std::string IP, int Port)
{
  try
  {
    // Set address
    if (enet_address_set_host(&Address, IP.c_str()) != 0) throw 0;
    Address.port = (enet_uint16)Port;

    // Connect  (set peer to server)
    Peer = enet_host_connect(Client, &Address, 1, 0);
    if (Peer == NULL) throw 1;

    return true;
  }
  catch(const int ErrorCode)
  {
    return false;
  }
}

void PollNetwork(entt::registry& Scene)
{
  while(enet_host_service(Client, &Event, 0) > 0)
  {
    switch(Event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
    {
      std::cout << "Connection event recieved" << "\n";
    }
    break;

    case ENET_EVENT_TYPE_RECEIVE:
    {
      uint8_t PacketHeader;
      memmove(&PacketHeader, Event.packet->data, 1);

      if (PacketHeader == 0)
      {
        // Set our self id and connection status
        OnConnection* Msg = (OnConnection*)Event.packet->data;
        SelfNetworkId = Msg->Nid;
        bConnected = true;
      }
      else if (PacketHeader == 1)
      {
        CreatePlayer* Msg = (CreatePlayer*)Event.packet->data;

        // Create player
        entt::entity Player = CreatePrefabPlayer(Scene, Msg->Nid, Msg->x, Msg->y,
          NetworkIdToPlayerAsset[(int)Msg->Nid]);

        // Handle slot
        NetworkClients[Msg->Nid].Id = Player;
        NetworkClients[Msg->Nid].NetworkId = Msg->Nid;
        NetworkClients[Msg->Nid].bActive = true;
      }
      else if (PacketHeader == 2)
      {
        DeletePlayer* Msg = (DeletePlayer*)Event.packet->data;

        // If no such player just gtfo
        if (!NetworkClients[Msg->Nid].bActive) break;

        entt::entity Player = NetworkClients[Msg->Nid].Id;

        // Remove shadow and entity
        Scene.destroy((entt::entity)Scene.get<Shadow>(Player).Reference);
        Scene.destroy(Player);

        // Handle slot
        NetworkClients[Msg->Nid].bActive = false;
      }
      else if (PacketHeader == 3)
      {
        UpdatePlayer* Msg = (UpdatePlayer*)Event.packet->data;

        // If no such player just gtfo
        if (!NetworkClients[Msg->Nid].bActive) break;

        // Apply updates
        entt::entity Player = NetworkClients[Msg->Nid].Id;
        auto& Pos = Scene.get<Position>(Player);

        Pos.x = Msg->x;
        Pos.y = Msg->y;
      }
      else if (PacketHeader == 4)
      {
        CreateBullet* Msg = (CreateBullet*)Event.packet->data;

        auto& Shk = Scene.get<Shake>(NetworkClients[(int)Msg->Nid].Id);
        Shk.Amount = 0.6f;

        CreatePrefabBullet(Scene, Msg->Nid, Msg->x, Msg->y, Msg->Direction,
          NetworkIdToBulletAsset[(int)Msg->Nid]);
      }
      else if (PacketHeader == 5)
      {
        DeathPlayer* Msg = (DeathPlayer*)Event.packet->data;

        auto& Shk = Scene.get<Shake>(NetworkClients[(int)Msg->Nid].Id);
        Shk.Amount = 0.0f;

        // Create Big smudge
        CreatePrefabBigSmudge(Scene, Msg->Nid, Msg->x, Msg->y);

        // Create smudge balls
        for (int i = 0; i < 5; i++)
        {
          CreatePrefabSmudgeBall(Scene, Msg->Nid, NetworkIdToBulletAsset[(int)Msg->Nid],
            Msg->x, Msg->y);
        }
      }

      enet_packet_destroy(Event.packet);
    }
    break;

    case ENET_EVENT_TYPE_DISCONNECT:
    {
      std::cout << "Disconnect event recieved boss" << "\n";

      // Set connection state
      SelfNetworkId = 255;
      bConnected = false;
    }
    break;
    }
  }
}

uint8_t GetSelfNetworkId()
{
  return SelfNetworkId;
}

entt::entity GetSelfPlayerEntity()
{
  return NetworkClients[SelfNetworkId].Id;
}

void SendMovement(bool bLeft, bool bRight, bool bUp, bool bDown)
{
  ClientMovementCommands Msg = {0, bLeft, bRight, bUp, bDown};
  ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
  enet_peer_send(Peer, 0, Packet);
}

void SendShooting(float Angle)
{
  ClientShootingCommands Msg = {1, Angle};
  ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
  enet_peer_send(Peer, 0, Packet);
}

void Disconnect()
{
  enet_peer_disconnect(Peer, 0);
}

void DeInitENet()
{
  enet_deinitialize();
}