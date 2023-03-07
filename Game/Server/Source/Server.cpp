#include <iostream>
#include <chrono>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <array>

#include "mingw.thread.h"
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

void ApplyNetworkInputToPlayer(entt::registry& Scene, entt::entity Player)
{
  auto& Inp = Scene.get<PlayerInput>(Player);

  /*
  Inp.bDown = Cmd->bDown;
  Inp.bUp = Cmd->bUp;
  Inp.bLeft = Cmd->bLeft;
  Inp.bRight = Cmd->bRight;
  */
}

entt::entity CreatePrefabPlayer(entt::registry& Scene, int NetworkId)
{
  entt::entity Player = Scene.create();
  Scene.emplace<NetId>(Player, NetworkId);
  Scene.emplace<Health>(Player, 0, 100);
  Scene.emplace<PlayerInput>(Player);
  Scene.emplace<Position>(Player, 40.0f, 32.0f);
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

        // Search for an empty slot
        for(int i = 0; i < NetworkClients.size(); i++)
        {
          // Skip active slots
          if (!NetworkClients[i].bActive)
          {
            // Empty slot is found, settle in
            NetworkClients[i].NetworkId = i;
            NetworkClients[i].bActive = true;
            NetworkClients[i].Id = CreatePrefabPlayer(Scene, i);

            // Send peer the self netid
            {
              int8_t Msg[2] = {0, (uint8_t)i};
              ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
              enet_peer_send(Event.peer, 0, Packet);
            }

            // Send every other peer info FOOR LOOP HEEREE
            {
              CreatePlayer Msg = {1, 0, 59, 50.5f, 74.9f};
              ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
              enet_peer_send(Event.peer, 0, Packet);
            }

            // broadcast self creation

            // Give data to peer
            Event.peer->data = &NetworkClients[i];
            NetworkClients[i].pPeer = Event.peer;
            break;
          }
        }

        // If didnt found an empty slot GTFO
        if (Event.peer->data == nullptr) enet_peer_disconnect(Event.peer, 0);
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

        if (PacketHeader == 0)
        {
          //Command* Cmd = (Command*)Event.packet->data;
          //ApplyNetworkInputToPlayer(Scene, PeerData->Id);
        }

        enet_packet_destroy(Event.packet);
      }
      break;

      case ENET_EVENT_TYPE_DISCONNECT:
      {
        std::cout << "Client disconnected: " << Event.peer << "\n"; 
        RemotePeer* PeerData = (RemotePeer*)Event.peer->data;
        PeerData->bActive = false;
        PeerData->pPeer = nullptr;
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