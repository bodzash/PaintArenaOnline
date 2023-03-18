#include <iostream>
#include <chrono>
#include <cstdint>
#include <array>
#include <thread>
#include <cstdlib>
#include <ctime>

#include "enet/enet.h"
#include "entt/entity/registry.hpp"

#include "Math.hpp"
#include "Components.hpp"
#include "NetworkTypes.hpp"
#include "Systems/BulletSystems.hpp"

using std::string;
using namespace std::chrono;

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

void DynamicMovementSystem(float DeltaTime, entt::registry& Scene)
{
  auto View = Scene.view<Position, Velocity, Speed>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Vel = View.get<Velocity>(Entity);
    auto& Spd = View.get<Speed>(Entity);

    float VelocityDirX = Vel.x;
    float VelocityDirY = Vel.y;

    NormalizeVector(VelocityDirX, VelocityDirY);

    Pos.x += VelocityDirX * abs(Vel.x) * DeltaTime;
    Pos.y += VelocityDirY * abs(Vel.y) * DeltaTime;

    Vel.x = Approach(Vel.x, 0.0f, Spd.Deceleration);
    Vel.y = Approach(Vel.y, 0.0f, Spd.Deceleration);
  }
}

void KeepPlayerInBoundsSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position, PlayerTag>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);

    if (Pos.x < 4.0f)
    {
      Pos.x = 4.0f;
    }
    else if (Pos.x > 236.0f)
    {
      Pos.x = 236.0f;
    }

    if (Pos.y < 4.0f)
    {
      Pos.y = 4.0f;
    }
    else if (Pos.y > 172.0f)
    {
      Pos.y = 172.0f;
    }  
  }
}

void NetworkPlayerUpdateSystem(entt::registry& Scene, ENetHost* Server)
{
  auto View = Scene.view<Position, NetworkId>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Nid = View.get<NetworkId>(Entity);

    // Send out info
    UpdatePlayer Msg = {3, (uint8_t)Nid.Id, Pos.x, Pos.y};
    ServerBroadcastMessage<UpdatePlayer>(Msg, Server);
  }
}

void InputToMovementSystem(entt::registry& Scene)
{
  auto View = Scene.view<PlayerInput, Velocity, Speed>();
  for (auto Entity : View)
  {
    auto& Inp = Scene.get<PlayerInput>(Entity);
    auto& Vel = View.get<Velocity>(Entity);
    auto& Spd = View.get<Speed>(Entity);

    // Input will serve as impulse force
    float InputX = Inp.bRight - Inp.bLeft;
    float InputY = Inp.bDown - Inp.bUp;

    // Apply impulse force to velocity
    Vel.x += InputX * std::min(Spd.Acceleration, Spd.MaxSpeed - abs(Vel.x));
    Vel.y += InputY * std::min(Spd.Acceleration, Spd.MaxSpeed - abs(Vel.y));;
  }
}

void ResetPlayerInputSystem(entt::registry& Scene)
{
  auto View = Scene.view<PlayerInput>();
  for (auto Entity : View)
  {
    auto& Inp = View.get<PlayerInput>(Entity);
    Inp = PlayerInput();
  }
}

void HealthDeathSystem(entt::registry& Scene, ENetHost* Server)
{
  auto View = Scene.view<Health, Position, NetworkId>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Hel = View.get<Health>(Entity);
    auto& Nid = View.get<NetworkId>(Entity);

    if (Hel.Current <= Hel.Min)
    {
      DeathPlayer Msg = {5, Nid.Id, Pos.x, Pos.y};
      ServerBroadcastMessage<DeathPlayer>(Msg, Server);

      Hel.Current = Hel.Max;
      Pos.x = (float)RandomRange(4, 236);
      Pos.y = (float)RandomRange(4, 172);
    }
  }
}

void ApplyNetworkInputToPlayer(entt::registry& Scene, entt::entity Player,
  ClientMovementCommands* Cmd)
{
  auto& Inp = Scene.get<PlayerInput>(Player);

  Inp.bDown = Cmd->bDown;
  Inp.bUp = Cmd->bUp;
  Inp.bLeft = Cmd->bLeft;
  Inp.bRight = Cmd->bRight;
}

entt::entity CreatePrefabPlayer(entt::registry& Scene, uint8_t NetId)
{
  entt::entity Player = Scene.create();
  Scene.emplace<PlayerTag>(Player);
  Scene.emplace<TeamId>(Player, NetId);
  Scene.emplace<NetworkId>(Player, NetId);
  Scene.emplace<Position>(Player, (float)RandomRange(4, 236), (float)RandomRange(4, 172));
  Scene.emplace<Velocity>(Player);
  Scene.emplace<Speed>(Player, 140.0f, 45.0f, 27.0f);
  Scene.emplace<Collider>(Player, 8.0f);
  Scene.emplace<Health>(Player, 0, 100);
  Scene.emplace<PlayerInput>(Player);

  return Player;
}

entt::entity CreatePrefabBullet(entt::registry& Scene, float x, float y, float Angle,
  uint8_t NetId)
{
  entt::entity Bullet = Scene.create();
  Scene.emplace<BulletTag>(Bullet);
  Scene.emplace<TeamId>(Bullet, NetId);
  Scene.emplace<Position>(Bullet, x, y);
  Scene.emplace<Direction>(Bullet, Angle);
  Scene.emplace<Collider>(Bullet, 4.0f);
  Scene.emplace<Speed>(Bullet, 140.0f);

  return Bullet;
}

// Program entry point
int main()
{
  // Set random seed value for random number generator
  srand((unsigned)time(NULL));

  // ECS world
  entt::registry Scene;

  // Timing
  const float FrameRate = 1000 / 60;
  float DeltaTime = 0.016f;
  system_clock::time_point TimeStart = system_clock::now();
  system_clock::time_point TimeEnd = system_clock::now();  

  // Scene and Network info
  const int MaxNetworkClients = 6;
  int NetworkClientNumber = 0;
  std::array<RemotePeer, MaxNetworkClients> NetworkClients;
  
  // ENet
  ENetHost* pServer;
  ENetAddress Address;
  ENetEvent Event;
  Address.host = ENET_HOST_ANY;
  Address.port = 7777;

  // Todo clean this mess
  if (enet_initialize() != 0) std::cout << "Init failed lol :D" << "\n";
  pServer = enet_host_create(&Address, 32, 1, 0, 0);
  if (pServer == NULL) std::cout << "Error when trying to create pServer host" << "\n";

  std::cout << "Game Server Started" << "\n";

  // Server Game Loop
  while (true)
  {
    // Delta timing and thread sleeping
    TimeStart = system_clock::now();
    duration<float, std::milli> WorkTime = TimeStart - TimeEnd;

    if (WorkTime.count() < FrameRate)
    {
      duration<float, std::milli> DeltaMs(FrameRate - WorkTime.count());
      milliseconds DeltaMsDuration = duration_cast<milliseconds>(DeltaMs);
      std::this_thread::sleep_for(milliseconds(DeltaMsDuration.count()));
    }

    TimeEnd = system_clock::now();
    duration<float, std::milli> sleep_time = TimeEnd - TimeStart;
    
    DeltaTime = (1.0f / (1000.0f / (WorkTime + sleep_time).count()));

    // Network polling
    while(enet_host_service(pServer, &Event, 0) > 0)
    {
      switch(Event.type)
      {
      case ENET_EVENT_TYPE_CONNECT:
      {
        std::cout << "Client connected: " << Event.peer->address.host << "\n";
        std::cout << "Peer: " << Event.peer << "\n";

        bool bFoundSlot = false;

        // Search for an empty slot
        for(int i = 0; i < NetworkClients.size(); i++)
        {
          // Skip active slots
          if (!NetworkClients[i].bActive)
          {
            // Empty slot is found, settle in
            bFoundSlot = true;
            NetworkClientNumber++;

            NetworkClients[i].NetworkId = (uint8_t)i;
            NetworkClients[i].bActive = true;
            NetworkClients[i].Id = CreatePrefabPlayer(Scene, i);

            // Send peer the self NetworkId
            {
              OnConnection Msg = {0, (uint8_t)i};
              ServerSendMessage<OnConnection>(Msg, Event.peer);
            }

            // Send every other peer info
            for(int j = 0; j < NetworkClients.size(); j++)
            {
              // Make sure we arent sending ourselves yet
              if (NetworkClients[j].bActive && NetworkClients[j].NetworkId != i)
              {
                auto& Pos = Scene.get<Position>(NetworkClients[j].Id);
                auto& Hel = Scene.get<Health>(NetworkClients[j].Id);

                CreatePlayer Msg = {1, (uint8_t)j, Pos.x, Pos.y};
                ServerSendMessage<CreatePlayer>(Msg, Event.peer);
              }
            }

            // Broadcast self creation
            {
              auto& Pos = Scene.get<Position>(NetworkClients[i].Id);
              auto& Hel = Scene.get<Health>(NetworkClients[i].Id);

              CreatePlayer Msg = {1, (uint8_t)i, Pos.x, Pos.y};
              ServerBroadcastMessage<CreatePlayer>(Msg, pServer);
            }

            // Give data to peer
            Event.peer->data = &NetworkClients[i];
            NetworkClients[i].pPeer = Event.peer;
            break;
          }
        }
        
        // If didnt found an empty slot GTFO
        if (!bFoundSlot) enet_peer_disconnect(Event.peer, 0);
      }
      break;

      case ENET_EVENT_TYPE_RECEIVE:
      {
        uint8_t PacketHeader;
        memmove(&PacketHeader, Event.packet->data, 1);
        RemotePeer* PeerData = (RemotePeer*)Event.peer->data;

        if (!PeerData->bActive) break;
        
        if (PacketHeader == 0)
        {
          ClientMovementCommands* Cmd = (ClientMovementCommands*)Event.packet->data;
          ApplyNetworkInputToPlayer(Scene, PeerData->Id, Cmd);
        }
        else if (PacketHeader == 1)
        {
          ClientShootingCommands* Cmd = (ClientShootingCommands*)Event.packet->data;

          auto& Pos = Scene.get<Position>(PeerData->Id);

          CreatePrefabBullet(Scene, Pos.x, Pos.y, Cmd->Angle, PeerData->NetworkId);
          
          CreateBullet Msg = {4, PeerData->NetworkId, Pos.x, Pos.y, Cmd->Angle};
          ServerBroadcastMessage<CreateBullet>(Msg, pServer);
        }

        enet_packet_destroy(Event.packet);
      }
      break;

      case ENET_EVENT_TYPE_DISCONNECT:
      {
        std::cout << "Client disconnected: " << Event.peer << "\n";

        // If peer is non player just break
        if (Event.peer->data == nullptr) break;

        // Decrement total
        NetworkClientNumber--;

        // Handle slot 
        RemotePeer* PeerData = (RemotePeer*)Event.peer->data;
        PeerData->bActive = false;
        PeerData->pPeer = nullptr;

        // Destroy entity
        Scene.destroy(PeerData->Id);

        // Send out a disconnect
        DeletePlayer Msg = {2, PeerData->NetworkId};
        ServerBroadcastMessage<DeletePlayer>(Msg, pServer);
      }
      break;
      }
    }

    // Update loop
    InputToMovementSystem(Scene);
    DynamicMovementSystem(DeltaTime, Scene);
    KeepPlayerInBoundsSystem(Scene);
    BulletMovementSystem(Scene, DeltaTime);
    ServerBulletDamageSystem(Scene);
    HealthDeathSystem(Scene, pServer);
    RemoveBulletOutOfBoundsSystem(Scene);

    // Post Update
    ResetPlayerInputSystem(Scene);
    NetworkPlayerUpdateSystem(Scene, pServer);
  }

  // Cleanup
  enet_host_destroy(pServer);
  enet_deinitialize();
}