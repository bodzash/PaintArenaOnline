#include <iostream>
#include "enet/enet.h"

#include "Net.h"

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

void PollNet(entt::registry& Scene)
{
  while(enet_host_service(Client, &Event, 0) > 0)
  {
    switch(Event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
      std::cout << "Connection event recieved, chief o7" << "\n"; 
      break;

    case ENET_EVENT_TYPE_RECEIVE:
      std::cout << Event.packet->data << "\n";



      enet_packet_destroy(Event.packet);
      break;
    }
  }
}

struct Command
{
  uint8_t Id = 0;
  bool Left;
  bool Right;
  bool Up;
  bool Down;
};

void SendPacket()
{
  string Msg = "Hi from client :)";
  ENetPacket* Packet = enet_packet_create(Msg.c_str(), strlen(Msg.c_str()) + 1,
    ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(Peer, 0, Packet);
}

void SendMovement(bool Left, bool Right, bool Up, bool Down)
{
  Command Msg = { 0, Left, Right, Up, Down };

  ENetPacket* Packet = enet_packet_create(&Msg, sizeof(Msg), 1);

  enet_peer_send(Peer, 0, Packet);

}

void Disconnect()
{
  enet_peer_disconnect(Peer, 0);
}