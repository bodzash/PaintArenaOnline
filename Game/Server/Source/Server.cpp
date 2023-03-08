#include <iostream>
#include <chrono>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <array>
#include <thread>

//#include "mingw.thread.h"
#include "enet/enet.h"
#include "entt/entity/registry.hpp"

#include "Math.hpp"
#include "Components.hpp"
#include "NetworkTypes.hpp"

using std::string;
using namespace std::chrono;

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

void ApplyNetworkInputToPlayer(entt::registry& Scene, entt::entity Player,
  ClientCommands* Cmd)
{
  auto& Inp = Scene.get<PlayerInput>(Player);

  Inp.bDown = Cmd->bDown;
  Inp.bUp = Cmd->bUp;
  Inp.bLeft = Cmd->bLeft;
  Inp.bRight = Cmd->bRight;
}

entt::entity CreatePrefabPlayer(entt::registry& Scene, uint8_t NetworkId)
{
  entt::entity Player = Scene.create();
  Scene.emplace<NetId>(Player, NetworkId);
  Scene.emplace<Health>(Player, 0, 100);
  Scene.emplace<PlayerInput>(Player);
  Scene.emplace<Position>(Player, 10.0f + RandomRange(0, 600),
    10.0f + RandomRange(0, 460));
  Scene.emplace<Velocity>(Player);
  Scene.emplace<Speed>(Player, 140.0f, 45.0f, 27.0f);

  return Player;
}

// CreatePrefabBullet()

int main()
{
  entt::registry Scene;

  const float FrameRate = 1000 / 60;
  float DeltaTime = 0.016f;
  system_clock::time_point TimeStart = system_clock::now();
  system_clock::time_point TimeEnd = system_clock::now();  

  const int MaxNetworkClients = 6;
  int NetworkClientNumber = 0;
  std::array<RemotePeer, MaxNetworkClients> NetworkClients;
  
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

  // pServer Game Loop
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

      //std::this_thread::sleep_for(milliseconds(DeltaMsDuration.count()));
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

            NetworkClients[i].NetworkId = i;
            NetworkClients[i].bActive = true;
            NetworkClients[i].Id = CreatePrefabPlayer(Scene, i);

            // Send peer the self netid
            {
              OnConnection Msg = {0, (uint8_t)i};
              ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
              enet_peer_send(Event.peer, 0, Packet);
            }

            // Send every other peer info
            for(int j = 0; j < NetworkClients.size(); j++)
            {
              // Make sure we arent sending ourselves yet
              if (NetworkClients[j].bActive && NetworkClients[j].NetworkId != i)
              {
                auto& Pos = Scene.get<Position>(NetworkClients[j].Id);
                auto& Hel = Scene.get<Health>(NetworkClients[j].Id);

                CreatePlayer Msg = {1, (uint8_t)j, Hel.Current, Pos.x, Pos.y};
                ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
                enet_peer_send(Event.peer, 0, Packet);
              }
            }

            // Broadcast self creation
            {
              auto& Pos = Scene.get<Position>(NetworkClients[i].Id);
              auto& Hel = Scene.get<Health>(NetworkClients[i].Id);

              CreatePlayer Msg = {1, (uint8_t)i, Hel.Current, Pos.x, Pos.y};
              ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
              enet_host_broadcast(pServer, 0, Packet);
            }

            // Give data to peer
            Event.peer->data = &NetworkClients[i];
            NetworkClients[i].pPeer = Event.peer;
            break;
          }
        }

        std::cout << "Total: " << NetworkClientNumber << "\n";
        std::cout << "Found: " << bFoundSlot << "\n";

        // If didnt found an empty slot GTFO
        if (!bFoundSlot) enet_peer_disconnect(Event.peer, 0);
      }
      break;

      case ENET_EVENT_TYPE_RECEIVE:
      {
        uint8_t PacketHeader;
        memmove(&PacketHeader, Event.packet->data, 1);
        RemotePeer* PeerData = (RemotePeer*)Event.peer->data;
        
        //std::cout << "Peer: " << Event.peer << "\n";
        //std::cout << "Header: " << (int)PacketHeader << "\n";
        //std::cout << "Peer Data: " << PeerData << "\n";

        if (!PeerData->bActive) break;
        
        if (PacketHeader == 0)
        {
          ClientCommands* Cmd = (ClientCommands*)Event.packet->data;
          ApplyNetworkInputToPlayer(Scene, PeerData->Id, Cmd);
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

        // Send out a disconnect
        DeletePlayer Msg = {2, PeerData->NetworkId};
        ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
        enet_host_broadcast(pServer, 0, Packet);
      }
      break;
      }
    }

    // Update loop
    InputToMovementSystem(Scene);
    DynamicMovementSystem(DeltaTime, Scene);
    ResetPlayerInputSystem(Scene);
    
    // For fast shutdown
    if (GetKeyState(VK_SPACE)) return 1; // TODO Remove this
  }

  // Cleanup
  enet_host_destroy(pServer);
  enet_deinitialize();
}