#include <iostream>
#include <cmath>

#include "enet/enet.h"

#include "Networking.hpp"
#include "Components.hpp"
#include "NetworkTypes.hpp"
#include "ClientTypes.hpp"

using std::string;

ENetAddress Address;
ENetEvent Event;
ENetPeer* Peer;
ENetHost* Client;

const int MaxNetworkClients = 6;
std::array<RemotePeer, MaxNetworkClients> NetworkClients;

bool bConnected = false;
uint8_t SelfNetworkId = 255;

bool IsConnected()
{
  return bConnected;
}

void ConnectToServer()
{
  if (enet_initialize() != 0) std::cout << "Init failed lol :D" << "\n";

  Client = enet_host_create(NULL, 1, 1, 0, 0);
  if (Client == NULL) std::cout << "Error when trying to create client host" << "\n";

  enet_address_set_host(&Address, "127.0.0.1");
  Address.port = 7777;

  Peer = enet_host_connect(Client, &Address, 1, 0);
  if (Peer == NULL) std::cout << "No available peer to initiate connection" << "\n";
}

void PollNetwork(entt::registry& Scene)
{
  while(enet_host_service(Client, &Event, 0) > 0)
  {
    switch(Event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
    {
      std::cout << "Connection event recieved chief" << "\n";
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
        entt::entity Player = Scene.create();
        Scene.emplace<PlayerTag>(Player);
        Scene.emplace<TeamId>(Player, Msg->Nid);
        Scene.emplace<NetworkId>(Player, Msg->Nid);
        Scene.emplace<Position>(Player, Msg->x, Msg->y);
        Scene.emplace<Collider>(Player, 8.0f);
        Scene.emplace<Audio>(Player, "Hit1", "Hit2", false, false);
        Scene.emplace<Sprite>(Player, NetworkIdToPlayerAsset[(int)Msg->Nid],
          4.0f, 4.0f, 4.0f, 4.0f);
        Scene.emplace<Shadow>(Player, "PlayerShadow");
        Scene.emplace<Shake>(Player);

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

        // Remove entity
        Scene.destroy(NetworkClients[Msg->Nid].Id);

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

        entt::entity Bullet = Scene.create();
        Scene.emplace<BulletTag>(Bullet);
        Scene.emplace<TeamId>(Bullet, Msg->Nid);
        Scene.emplace<Position>(Bullet, Msg->x, Msg->y);
        Scene.emplace<Direction>(Bullet, Msg->Direction);
        Scene.emplace<Speed>(Bullet, 140.0f);
        Scene.emplace<Collider>(Bullet, 4.0f);
        Scene.emplace<Audio>(Bullet, "Shoot1", "Shoot2", true, true);
        Scene.emplace<Sprite>(Bullet, NetworkIdToBulletAsset[(int)Msg->Nid], 2.0f, 2.0f);
        Scene.emplace<Shadow>(Bullet, "BulletShadow");
      } else if (PacketHeader == 5)
      {
        DeathPlayer* Msg = (DeathPlayer*)Event.packet->data;

        auto& Shk = Scene.get<Shake>(NetworkClients[(int)Msg->Nid].Id);
        Shk.Amount = 0.0f;

        // Create Big smudge
        int Index = rand() % 3;
        string Asset = "BigSmudge1";
        if (Index == 1) Asset = "BigSmudge2";
        else if (Index == 2) Asset = "BigSmudge3";

        entt::entity Effect = Scene.create();
        Scene.emplace<Position>(Effect, Msg->x, Msg->y);
        Scene.emplace<Smudge>(Effect, Asset, (int)Msg->Nid);

        // Create smudge balls
        for (int i = 0; i < 5; i++)
        {
          entt::entity Ball = Scene.create();
          Scene.emplace<Position>(Ball, Msg->x, Msg->y);
          Scene.emplace<Direction>(Ball, (float)(rand() % 628) / 100.0f);
          Scene.emplace<Speed>(Ball, (float)((rand() % 130 - 80) + 80));
          Scene.emplace<TeamId>(Ball, Msg->Nid);
          Scene.emplace<Sprite>(Ball, NetworkIdToBulletAsset[(int)Msg->Nid], 2.0f, 2.0f);
          Scene.emplace<Shadow>(Ball, "BulletShadow");
          Scene.emplace<SmudgeBall>(Ball, 5.0f + (float)(rand() % 10));
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