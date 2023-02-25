#include <iostream>
#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Net.h"

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

void SpriteRendererSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);

    DrawCircle(Pos.x, Pos.y, 16.0f, RED);
  }
}

int main(void)
{
  entt::registry Scene;

  entt::entity Player = Scene.create();
  Scene.emplace<NetId>(Player, 0);
  Scene.emplace<Position>(Player, 0, 0);

  InitWindow(640, 480, "Game Client");
  SetTargetFPS(60);

  ConnectToServer();
  
  while (!WindowShouldClose())
  {
    PollNet(Scene);

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
      SendMovement(IsKeyDown(KEY_A), IsKeyDown(KEY_D),
        IsKeyDown(KEY_W), IsKeyDown(KEY_S));
    }

    BeginDrawing();
    ClearBackground({167, 167, 167, 255});

    SpriteRendererSystem(Scene);
    
    EndDrawing();
  }
  CloseWindow();
  
  return 0;
}

/*
#include <cstdio>
#include <cstring>
#include <malloc/_malloc.h>
#include <stdio.h>
#include <iostream>
 
typedef uint8_t *pointer;
 
struct Header {
    uint8_t kind = 0;
    Header() {
        kind = 0;
    }
    Header(int kind) {
        this->kind = kind;
    }
};
 
struct PacketA {
    const static int KIND_ID = 1;
    Header header;
    int field_a;
    int field_b;
};
 
struct PacketB {
    const static int KIND_ID = 2;
    Header header;
    char field[12];
};
 
int main() {
 
    // init a new packet
    PacketA p = {
        Header(PacketA::KIND_ID),
        100,
        200
    };
 
    // allocate our buffer which holds a stream of packets
    pointer buffer = (pointer)malloc(1000);
 
    // copy our packet to buffer
    memmove(buffer, &p, sizeof(p));
 
    // copy the header from the stream
    Header h;
    memmove(&h, buffer, sizeof(h));
    printf("got header %d\n", h.kind);
 
    // once we know the packet kind we can copy the full packet
    if (h.kind == PacketA::KIND_ID) {
        PacketA p;
        memmove(&p, buffer, sizeof(p));
        printf("Header kind: %d  with a = %d, b = %d\n", p.header.kind, p.field_a, p.field_b);
    }
 
    return 0;
}
*/

/*
struct Cum
{
  uint8_t load;
  int amount;
};

int main()
{
    Cum c = { 69, 4200 };
    
    
    uint8_t holder;
    
    memmove(&holder, &c, 1);
    
    cout << (int)holder;

    return 0;
}
*/