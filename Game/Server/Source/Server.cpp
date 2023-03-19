#include <iostream>
#include <chrono>
#include <cstdint>
#include <array>
#include <thread>
#include <cstdlib>

#include "enet/enet.h"
#include "entt/entity/registry.hpp"

#include "Math.hpp"
#include "Components.hpp"
#include "NetworkTypes.hpp"
#include "SvNetworking.hpp"
#include "Systems/BulletSystems.hpp"
#include "Systems/MovementSystems.hpp"
#include "Systems/BoundsSystems.hpp"
#include "Systems/HealthSystems.hpp"
#include "Systems/SvInputSystems.hpp"
#include "Systems/SvNetworkSystems.hpp"

using namespace std::chrono;

// Program entry point
int main()
{
  // Set random seed value for random number generator
  SetRandomSeed();

  // ECS world
  entt::registry Scene;

  // Timing
  const float FrameRate = 1000 / 60;
  float DeltaTime = 0.016f;
  system_clock::time_point TimeStart = system_clock::now();
  system_clock::time_point TimeEnd = system_clock::now();  

  // Network info
  std::array<RemotePeer, 6> NetworkClients;
  
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
        HandleClientConnect(Event, Scene, pServer, NetworkClients);
      break;

      case ENET_EVENT_TYPE_RECEIVE:
        HandleServerReceive(Event, Scene, pServer);
      break;

      case ENET_EVENT_TYPE_DISCONNECT:
        HandleClientDisconnect(Event, Scene, pServer);
      break;
      }
    }

    // Update loop
    InputToMovementSystem(Scene);
    DynamicMovementSystem(DeltaTime, Scene);
    KeepPlayerInBoundsSystem(Scene);
    DirectionalMovementSystem(Scene, DeltaTime);
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