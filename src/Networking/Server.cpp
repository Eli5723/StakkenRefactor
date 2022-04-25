#include <Networking/Server.h>

#include <Networking/Peer.h>
#include <Networking/Room.h>

#include <Networking/Message.h>

#include <rigtorp/SPSCQueue.h>
#include <SFMLHelper.h>

#include <enet/enet.h>

#include <map>
#include <string_view>
#include <sstream>
#include <chrono>
#include <thread>


namespace Network {

std::map<int, Room*> rooms;
std::map<int, Peer*> clients;

ENetAddress address;
ENetHost * server;

rigtorp::SPSCQueue<sf::Packet> async_queue(10);

void send_async(sf::Packet&& packet){
    async_queue.push(std::move(packet));
}

void send(const sf::Packet& packet, ENetPeer* peer, const u8 channel = 0)
{
  ENetPacket* epacket = enet_packet_create(packet.getData(), packet.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(peer, channel, epacket);
}

// // Rooms
// Room* room_create(const std::string name, int creatorId, int gamemode){
//     Room* newRoom = new Room;
//     newRoom->info.name = name;
//     newRoom->info.id = rand(); // TODO: Replace

//     printf("ROOM | create | %s\n", newRoom->info.name.c_str());

//     rooms[newRoom->info.id] = newRoom;
//     return newRoom;
// }

// void room_remove(Room* room){
//     rooms.erase(room->info.id);

//     printf("ROOM | Remove | %s\n", room->info.name.c_str());

//     delete room;
// }

// // Clients
// Peer* client_create(const Profile& profile){
//     Peer* newClient = new Peer;
//     newClient->profile = profile;
//     newClient->id = rand(); // TODO: Replace
//     printf("Peer | Login | %s\n", newClient->profile.name.c_str());
    
//     clients[newClient->id] = newClient;
//     return newClient;
// }

// Peer* client_create_guest(const std::string_view& name){
//     Peer* newClient = new Peer;
//     newClient->id = rand(); // TODO: Replace
//     // newClient->profile.id = -1;
//     // newClient->profile.roles = -1;
//     // newClient->profile.name = name;

//     printf("CLIENT | Login | %s (Guest)\n", newClient->profile.name.c_str());

//     clients[newClient->id] = newClient;
//     return newClient;
// }

// void client_remove(Client* client){
//     clients.erase(client->id);

//     // TODO: Remove Client from room
//     printf("CLIENT | Disconnect | %s\n", client->profile.name.c_str());
//     delete client;
// }

// Broadcasting
// void room_list(SendBuffer& buffer){
//     buffer.Write<int>(rooms.size());

//     for (const auto [id, room] : rooms){
//         buffer.Serialize<RoomInfo>(room->info);
//     }
// }

// void player_list(SendBuffer& buffer){
//     buffer.Write<int>(clients.size());

//     for (const auto [id, client] : clients){
//         buffer.Serialize<Profile>(client->profile);
//     }
// }


// void send_login_reject(SendBuffer& sendBuffer, ENetPeer* peer, std::string reason){
//     sendBuffer.Begin(Message::LOGIN_FAIL);
//     sendBuffer.WriteString("User accounts have not been implemeted yet.");
//     send_buffer(peer, sendBuffer);
//     sendBuffer.Reset();
// }

// void send_login_success(SendBuffer& sendBuffer, ENetPeer* peer){
//     sendBuffer.Begin(Message::LOGIN_SUCCESS);
//     room_list(sendBuffer);
//     player_list(sendBuffer);
//     send_buffer(peer, sendBuffer);
// }


// Packet Handling
void OnData(ENetPeer* peer, sf::Packet& packet){
    Message m;

    packet >> m;

    switch (m) {

           case Message::LOGIN_GUEST: {
                std::string nickname;
                packet >> nickname;

                Client* client = new  Client;

                client->id = -1;
                client->name = nickname;
                client->roles = Roles::GUEST;
                client->socket = peer;
                peer->data = client;

                // Inform Client
                sf::Packet pk;
               
                pk << Message::LOGIN_SUCCESS;
                pk << client->id;
                pk << client->roles;
                pk << client->name;

                send(pk, peer);

                printf("Got guest login: %s\n", nickname.c_str());
            } break;

            case Message::LOGIN_ACCOUNT: {
                std::string username;
                std::string password;

                packet >> username;
                packet >> password;

                Client* client = new  Client;
                // TODO: add accounts
                client->id = -1;
                client->name = username;
                client->roles = Roles::GUEST;
                client->socket = peer;
                peer->data = client;

                // Inform Client
                sf::Packet pk;
               
                pk << Message::LOGIN_SUCCESS;
                pk << client->id;
                pk << client->roles;
                pk << client->name;

                send(pk, peer);

                printf("Got login: %s | %s\n", username.c_str(), password.c_str());
            } break;
    
        default:
            puts("Recieved Invalid Message from client.");
            // TODO: Kick Client
        break;
    }
}


void service(){
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    server = enet_host_create(&address, 32, 2, 0, 0);

    if (server == NULL)
    {
        fprintf (stderr, "An error occurred while trying to create an ENet server host.\n");
        exit (EXIT_FAILURE);
    }

    ENetEvent event;
    while (true){
        auto start = std::chrono::high_resolution_clock::now();
        
        while (enet_host_service(server, &event, 0) > 0){
        switch (event.type){
            case ENET_EVENT_TYPE_CONNECT: {
            } break;
            
            case ENET_EVENT_TYPE_RECEIVE:{
                sf::Packet incoming;
                incoming.append(event.packet->data,event.packet->dataLength);
                OnData(event.peer, incoming);
                enet_packet_destroy (event.packet);
            } break;

            case ENET_EVENT_TYPE_DISCONNECT: {
                Client* client = (Client*)(event.peer->data);

                printf("User has disconnected: %s\n", client->name.c_str());
                delete client;
                
            } break;
        }}
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = start - end;
        
        enet_host_flush(server);
        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }

    enet_host_destroy(server);
}

}


int main() {
    Network::service();

    return 0;
}