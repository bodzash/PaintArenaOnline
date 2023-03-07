#include <iostream>
#include "enet/enet.h"

#include "Network.hpp"
#include "Components.hpp"
#include "NetworkTypes.hpp"

using std::string;

ENetAddress Address;
ENetEvent Event;
ENetPeer* Peer;
ENetHost* Client;

void ConnectToServer()
{
  if (enet_initialize() != 0) std::cout << "Init failed lol :D" << "\n";

  Client = enet_host_create(NULL, 1, 1, 0, 0);
  if (Client == NULL) std::cout << "Error when trying to create client host" << "\n";

  enet_address_set_host(&Address, "127.0.0.1");
  Address.port = 7777;

  Peer = enet_host_connect(Client, &Address, 1, 0);
  if (Peer == NULL) std::cout << "No available peer to initiate connection" << "\n";
}

void PollNetwork(entt::registry& Scene)
{
  while(enet_host_service(Client, &Event, 0) > 0)
  {
    switch(Event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
    {
      std::cout << "Connection event recieved chief" << "\n";
      //set bConnected to trueeee 
    }
    break;

    case ENET_EVENT_TYPE_RECEIVE:
    {
      uint8_t PacketHeader;
      memmove(&PacketHeader, Event.packet->data, 1);
      std::cout << "Header: " << (int)PacketHeader << "\n";

      if (PacketHeader == 0)
      {
        std::cout << (int)Event.packet->data[1] << "\n";
        // set self id to msg
      }
      else if (PacketHeader == 1)
      {
        std::cout << "ajlenbojj" << "\n";

        CreatePlayer* Msg = (CreatePlayer*)Event.packet->data;

        entt::entity Player = Scene.create();
        Scene.emplace<NetId>(Player, Msg->Nid);
        Scene.emplace<Position>(Player, Msg->x, Msg->y);
        Scene.emplace<Health>(Player, 0, 100, Msg->Health);
      }

      enet_packet_destroy(Event.packet);
    }
    break;

    case ENET_EVENT_TYPE_DISCONNECT:
    {
      std::cout << "Disconnect event recieved boss" << "\n";
      //set bConnected to falseeeeee
    }
    break;
    }
  }
}

void SendPacket()
{
  string Msg = "Hi from client :)";
  ENetPacket* Packet = enet_packet_create(Msg.c_str(), strlen(Msg.c_str()) + 1,
    ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(Peer, 0, Packet);
}

void SendMovement(bool Left, bool Right, bool Up, bool Down)
{
  ClientCommands Msg = { Left, Right, Up, Down };

  ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);

  enet_peer_send(Peer, 0, Packet);
}

void Disconnect()
{
  enet_peer_disconnect(Peer, 0);
  enet_deinitialize();
}