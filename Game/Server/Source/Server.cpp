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

using std::string;

std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

const float FrameRate = 1000 / 60;
float DeltaTime = 0.016f;

void Broadcast(ENetHost* Server)
{
  string Msg = "Sup from the Server";
  ENetPacket* Packet = enet_packet_create(Msg.c_str(), strlen(Msg.c_str()) + 1,
    ENET_PACKET_FLAG_RELIABLE);
  enet_host_broadcast(Server, 0, Packet);
}

struct RemotePeer
{
  int Id;
  int NetworkId;
  bool bActive;
  ENetPeer* pPeer;
};

struct PlayerUpdate
{
  uint8_t Id;
  uint8_t NetworkId;
  float x;
  float y;
};

struct Command
{
  uint8_t Id = 0;
  bool bLeft;
  bool bRight;
  bool bUp;
  bool bDown;
};

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

void ApplyNetworkInputToPlayer(entt::registry& Scene, entt::entity Player, Command* Cmd)
{
  auto& Inp = Scene.get<PlayerInput>(Player);

  Inp.bDown = Cmd->bDown;
  Inp.bUp = Cmd->bUp;
  Inp.bLeft = Cmd->bLeft;
  Inp.bRight = Cmd->bRight;
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

entt::entity CreatePlayer(entt::registry& Scene, int NetworkId)
{
  entt::entity Player = Scene.create();
  Scene.emplace<NetId>(Player, NetworkId);
  Scene.emplace<PlayerInput>(Player);
  Scene.emplace<Position>(Player, 40.0f, 32.0f);
  Scene.emplace<Velocity>(Player);
  Scene.emplace<Speed>(Player, 140.0f, 45.0f, 27.0f);

  return Player;
}

void Placeholder()
{
  //
}

int main()
{
  entt::registry Scene;

  const int MaxNetworkClients = 6;
  std::array<RemotePeer, MaxNetworkClients> NetworkClients;

  CreatePlayer(Scene, 0);
  
  if (enet_initialize() != 0) std::cout << "Init failed lol :D" << "\n";

  ENetHost* Server;
  ENetAddress Address;
  ENetEvent Event;
  Address.host = ENET_HOST_ANY;
  Address.port = 7777;

  // Todo clean this mess
  Server = enet_host_create(&Address, 32, 1, 0, 0);
  if (Server == NULL) std::cout << "Error when trying to create server host" << "\n";

  std::cout << "GameServer Started" << "\n";

  // Server Game Loop
  while (true)
  {
    // Timing bs TODO CLEANUP
    a = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> work_time = a - b;

    if (work_time.count() < FrameRate)
    {
      std::chrono::duration<double, std::milli> delta_ms(FrameRate - work_time.count());
      std::chrono::milliseconds delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
      std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
    }

    b = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> sleep_time = b - a;

    float DeltaTime = 1 / (1000 / (work_time + sleep_time).count());

    // Network polling
    while(enet_host_service(Server, &Event, 0) > 0)
    {
      switch(Event.type)
      {
      case ENET_EVENT_TYPE_CONNECT:
      {
        std::cout << "Client connected: " << Event.peer->address.host << "\n";
        std::cout << "Peer: " << Event.peer << "\n";

        //Event.peer->data = &NetworkClients[0];

        // Send something to the currently connected peer
        uint8_t Msg[2] = {0, 0};
        ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
        enet_peer_send(Event.peer, 0, Packet);
        
        //std::cout << "Peer Data: " << *(uint32_t*)Event.peer->data << "\n";
        }
      break;

      case ENET_EVENT_TYPE_RECEIVE:
      {
        //uint32_t PeerData = *(uint32_t*)Event.peer->data;
        uint8_t PacketHeader;
        memmove(&PacketHeader, Event.packet->data, 1);
        
        //std::cout << "Peer: " << Event.peer << "\n";
        //std::cout << "Header: " << (int)PacketHeader << "\n";
        //std::cout << "Peer Data: " << PeerData << "\n";

        if (PacketHeader == 0)
        {
          Command* Cmd = (Command*)Event.packet->data;
          ApplyNetworkInputToPlayer(Scene, /*NetworkClients[0]*/(entt::entity)0, Cmd);
        }

        enet_packet_destroy(Event.packet);
      }
      break;

      case ENET_EVENT_TYPE_DISCONNECT:
      {
        std::cout << "Client disconnected" << "\n"; 
      }
      break;
      }
    }

    // Update loop
    InputToMovementSystem(Scene);
    DynamicMovementSystem(DeltaTime, Scene);
    ResetPlayerInputSystem(Scene);

    // Move this shit TODO Cleanup
    /*
    auto& Pos = Scene.get<Position>((entt::entity)0);
    PlayerUpdate Msg = { 0, 0, Pos.x, Pos.y };
    ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);
    enet_host_broadcast(Server, 0, Packet);
    */
    
    // For fast shutdown
    if (GetKeyState(VK_SPACE)) return 1; // TODO Remove this
  }

  // Cleanup
  enet_host_destroy(Server);
  enet_deinitialize();
}


/*
int main()
{
  using clock = std::chrono::steady_clock;
  using time_point = std::chrono::_V2::steady_clock::time_point;

  time_point next_frame = clock::now();

  while(true)
  {
    // Time Start bullshit
    next_frame += std::chrono::milliseconds(1000 / 60);

    // Network Poll
    // Update

    std::cout << 0.016f << '\n';

    // End of Frame
    std::this_thread::sleep_until(next_frame);
  }
}
*/