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
  std::array<RemotePeer, 6> NetworkClients;
  float DeltaTime = 0.016f;
  
  // ENet setup
  ENetHost* pServer;
  ENetEvent Event;
  ENetAddress Address = {ENET_HOST_ANY, 7777};
  
  // Init
  if (!InitServerHost(pServer, Address)) return EXIT_FAILURE;

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
  DeinitServerHost(pServer);
}