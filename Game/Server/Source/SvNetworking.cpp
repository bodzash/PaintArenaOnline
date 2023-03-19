#include "SvNetworking.hpp"

void HandleClientConnect(ENetEvent& Event, entt::registry& Scene, ENetHost* pServer,
  std::array<RemotePeer, 6>& NetworkClients)
{
  std::cout << "Client connected: " << Event.peer->address.host << "\n";
  std::cout << "Peer: " << Event.peer << "\n";

  bool bFoundSlot = false;

  // Search for an empty slot
  for(int i = 0; i < NetworkClients.size(); i++)
  {
    // Skip active slots
    if (!NetworkClients[i].bActive)
    {
      // Empty slot is found, settle in
      bFoundSlot = true;

      NetworkClients[i].NetworkId = (uint8_t)i;
      NetworkClients[i].bActive = true;
      NetworkClients[i].Id = CreatePrefabPlayer(Scene, i);

      // Send peer the self NetworkId
      {
        OnConnection Msg = {0, (uint8_t)i};
        ServerSendMessage<OnConnection>(Msg, Event.peer);
      }

      // Send every other peer info
      for(int j = 0; j < NetworkClients.size(); j++)
      {
        // Make sure we arent sending ourselves yet
        if (NetworkClients[j].bActive && NetworkClients[j].NetworkId != i)
        {
          auto& Pos = Scene.get<Position>(NetworkClients[j].Id);
          auto& Hel = Scene.get<Health>(NetworkClients[j].Id);

          CreatePlayer Msg = {1, (uint8_t)j, Pos.x, Pos.y};
          ServerSendMessage<CreatePlayer>(Msg, Event.peer);
        }
      }

      // Broadcast self creation
      {
        auto& Pos = Scene.get<Position>(NetworkClients[i].Id);
        auto& Hel = Scene.get<Health>(NetworkClients[i].Id);

        CreatePlayer Msg = {1, (uint8_t)i, Pos.x, Pos.y};
        ServerBroadcastMessage<CreatePlayer>(Msg, pServer);
      }

      // Give data to peer
      Event.peer->data = &NetworkClients[i];
      NetworkClients[i].pPeer = Event.peer;
      break;
    }
  }
  
  // If didnt found an empty slot GTFO
  if (!bFoundSlot) enet_peer_disconnect(Event.peer, 0);
}

void HandleClientDisconnect(ENetEvent& Event, entt::registry& Scene, ENetHost* pServer)
{
  std::cout << "Client disconnected: " << Event.peer << "\n";

  // If peer is non player just break
  if (Event.peer->data == nullptr) return;

  // Handle slot 
  RemotePeer* PeerData = (RemotePeer*)Event.peer->data;
  PeerData->bActive = false;
  PeerData->pPeer = nullptr;

  // Destroy entity
  Scene.destroy(PeerData->Id);

  // Send out a disconnect
  DeletePlayer Msg = {2, PeerData->NetworkId};
  ServerBroadcastMessage<DeletePlayer>(Msg, pServer);
}

void HandleServerReceive(ENetEvent& Event, entt::registry& Scene, ENetHost* pServer)
{
  uint8_t PacketHeader;
  memmove(&PacketHeader, Event.packet->data, 1);
  RemotePeer* PeerData = (RemotePeer*)Event.peer->data;

  if (!PeerData->bActive) return;
  
  if (PacketHeader == 0)
  {
    ClientMovementCommands* Cmd = (ClientMovementCommands*)Event.packet->data;
    ApplyNetworkInputToPlayer(Scene, PeerData->Id, Cmd);
  }
  else if (PacketHeader == 1)
  {
    ClientShootingCommands* Cmd = (ClientShootingCommands*)Event.packet->data;

    auto& Pos = Scene.get<Position>(PeerData->Id);

    CreatePrefabBullet(Scene, Pos.x, Pos.y, Cmd->Angle, PeerData->NetworkId);
    
    CreateBullet Msg = {4, PeerData->NetworkId, Pos.x, Pos.y, Cmd->Angle};
    ServerBroadcastMessage<CreateBullet>(Msg, pServer);
  }

  enet_packet_destroy(Event.packet);
}