#include "Systems/SvInputSystems.hpp"

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
  ClientMovementCommands* Cmd)
{
  auto& Inp = Scene.get<PlayerInput>(Player);

  Inp.bDown = Cmd->bDown;
  Inp.bUp = Cmd->bUp;
  Inp.bLeft = Cmd->bLeft;
  Inp.bRight = Cmd->bRight;
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