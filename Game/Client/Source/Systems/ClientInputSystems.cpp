#include "Systems/ClientInputSystems.hpp"

void PlayerNetworkInputSystem(entt::registry& Scene)
{
  if (IsConnected())
    {
    entt::entity MyPlayerId = GetSelfPlayerEntity();
    uint8_t MyNetworkId = GetSelfNetworkId();
    bool bLeft = IsKeyDown(KEY_A) ;
    bool bRight = IsKeyDown(KEY_D);
    bool bUp = IsKeyDown(KEY_W);
    bool bDown = IsKeyDown(KEY_S);

    // Calculate angle
    if (MyNetworkId != 255)
    {
      auto& Pos = Scene.get<Position>(MyPlayerId);
      float Angle = PointDirection(Pos.x, Pos.y, GetMouseX() / 4, GetMouseY() / 4);
      
      if (bLeft || bRight || bUp || bDown) SendMovement(bLeft, bRight, bUp, bDown);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) SendShooting(Angle);
    }
  }
}