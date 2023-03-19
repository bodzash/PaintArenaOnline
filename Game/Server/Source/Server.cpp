#include <iostream>
#include <cstdint>
#include <array>

#include "enet/enet.h"
#include "entt/entity/registry.hpp"

#include "Math.hpp"
#include "Timing.hpp"
#include "Components.hpp"
#include "NetworkTypes.hpp"
#include "SvNetworking.hpp"
#include "Systems/BulletSystems.hpp"
#include "Systems/MovementSystems.hpp"
#include "Systems/BoundsSystems.hpp"
#include "Systems/HealthSystems.hpp"
#include "Systems/SvInputSystems.hpp"
#include "Systems/SvNetworkSystems.hpp"

// Program entry point
int main()
{  
  // Setup
  SetRandomSeed();
  entt::registry Scene;
  float DeltaTime = 0.016f;
  std::array<RemotePeer, 6> NetworkClients;
  
  // ENet
  ENetHost* pServer;
  ENetAddress Address;
  ENetEvent Event;
  Address.host = ENET_HOST_ANY;
  Address.port = 7777;

  // Init TODO clean this mess
  if (enet_initialize() != 0) std::cout << "Init failed lol :D" << "\n";
  pServer = enet_host_create(&Address, 32, 1, 0, 0);
  if (pServer == NULL) std::cout << "Error when trying to create pServer host" << "\n";

  std::cout << "Game Server Started" << "\n";

  // Server Game Loop
  while (true)
  {
    // Delta timing and thread sleeping
    DeltaTime = CalculateDeltaTimeAndSleepThread();

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