#include "Systems/HealthSystems.hpp"

void HealthDeathSystem(entt::registry& Scene, ENetHost* Server)
{
  auto View = Scene.view<Health, Position, NetworkId>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Hel = View.get<Health>(Entity);
    auto& Nid = View.get<NetworkId>(Entity);

    if (Hel.Current <= Hel.Min)
    {
      DeathPlayer Msg = {5, Nid.Id, Pos.x, Pos.y};
      ServerBroadcastMessage<DeathPlayer>(Msg, Server);

      Hel.Current = Hel.Max;
      Pos.x = (float)RandomRange(4, 236);
      Pos.y = (float)RandomRange(4, 172);
    }
  }
}