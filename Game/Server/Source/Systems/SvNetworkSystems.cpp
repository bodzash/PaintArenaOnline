#include "Systems/SvNetworkSystems.hpp"

void NetworkPlayerUpdateSystem(entt::registry& Scene, ENetHost* Server)
{
  auto View = Scene.view<Position, NetworkId>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Nid = View.get<NetworkId>(Entity);

    // Send out info
    UpdatePlayer Msg = {3, (uint8_t)Nid.Id, Pos.x, Pos.y};
    ServerBroadcastMessage<UpdatePlayer>(Msg, Server);
  }
}