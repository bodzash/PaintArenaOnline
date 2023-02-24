#include <iostream>
#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Net.h"

entt::registry Registry;

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  SetTargetFPS(60);

  Connect();
  
  while (!WindowShouldClose())
  {
    PollNet();

    if (IsKeyPressed(KEY_SPACE)) SendPacket();

    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
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