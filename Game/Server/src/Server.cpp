#include <iostream>
#include <chrono>
#include <cstdint>
#include <cmath>
#include <map>
#include "mingw.thread.h"

#include "enet/enet.h"
#include "entt/entity/registry.hpp"

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

struct Position
{
  float x = 0.0f;
  float y = 0.0f;
};

struct Velocity
{
  float x = 0.0f;
  float y = 0.0f;
};

struct Speed
{
  float MaxSpeed = 0.0f;
  float Acceleration = 0.0f;
  float Deceleration = 0.0f;
};

struct NetId
{
  int Id;
};

void NormalizeVector(float& x, float& y)
{
  float m = std::max(sqrtf(x * x + y * y), 1.0f);
  x /= m;
  y /= m;
}

float Approach(float Start, float End, float Shift)
{
  if (Start < End)
  {
    return std::min(Start + Shift, End);
  }
  else
  {
    return std::max(Start - Shift, End);
  }
}

/*
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
*/

int main()
{
  entt::registry Scene;

  // Test entity
  entt::entity Player = Scene.create();
  Scene.emplace<NetId>(Player, 0);
  Scene.emplace<Position>(Player, 40.0f, 32.0f);
  Scene.emplace<Velocity>(Player);
  Scene.emplace<Speed>(Player, 140.0f, 45.0f, 27.0f);
  

  if (enet_initialize() != 0) std::cout << "Init failed lol :D" << "\n";

  ENetHost* Server;
  ENetAddress Address;
  ENetEvent Event;
  Address.host = ENET_HOST_ANY;
  Address.port = 7777;

  Server = enet_host_create(&Address, 32, 1, 0, 0);
  if (Server == NULL) std::cout << "Error when trying to create server host" << "\n";

  std::cout << "GameServer Started" << "\n";

  // Server Game Loop
  while (true)
  {
    // Maintain 60 FPS
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
        std::cout << "Client connected: " << Event.peer->address.host << "\n";

        std::cout << "Peer: " << Event.peer << "\n";

        Broadcast(Server);
        break;

      case ENET_EVENT_TYPE_RECEIVE:
        std::cout << "Message: " << Event.packet->data << "\n";
        //std::cout << "Packet: " << Event.packet << "\n";
        std::cout << "Peer: " << Event.peer << "\n";

        enet_packet_destroy(Event.packet);
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        std::cout << "Client disconnected" << "\n"; 
        break;
      }
    }

    // Update
    //DynamicMovementSystem(DeltaTime, Scene);
    if (GetKeyState('A')) return 1;

    //std::cout << DeltaTime << "\n";
  }

  // Cleanup
  enet_host_destroy(Server);
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

    std::cout << 0.0016f << '\n';

    // End of Frame
    std::this_thread::sleep_until(next_frame);
  }
}
*/