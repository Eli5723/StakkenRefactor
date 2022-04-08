#include <Networking/Server.h>

#include <Networking/Room.h>
#include <Networking/Client.h>
#include <Networking/Message.h>
#include <Networking/SendBuffer.h>
#include <Networking/ReadBuffer.h>

#include <enet/enet.h>

#include <map>
#include <string_view>
#include <sstream>
#include <chrono>
#include <thread>

namespace Network {


std::map<int, Room*> rooms;
std::map<int, Client*> clients;

ENetAddress address;
ENetHost * server;

inline void send_buffer(ENetPeer* peer, SendBuffer& buffer){
    ENetPacket* packet = enet_packet_create(buffer.GetBuffer(), buffer.GetBufferSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(server);
}

// Rooms
Room* room_create(const std::string_view& name, int creatorId, int gamemode){
    Room* newRoom = new Room;
    newRoom->info.name = name;
    newRoom->info.id = rand(); // TODO: Replace

    printf("Created new room: %s\n", newRoom->info.name.c_str());

    rooms[newRoom->info.id] = newRoom;
    return newRoom;
}

void room_remove(Room* room){
    rooms.erase(room->info.id);

    delete room;
}

// Clients
Client* client_create(const Profile& profile){
    Client* newClient = new Client;
    newClient->profile = profile;
    newClient->id = rand(); // TODO: Replace
    printf("Player login: %s\n", newClient->profile.name.c_str());
    
    clients[newClient->id] = newClient;
    return newClient;
}

Client* client_create_guest(const std::string_view& name){
    Client* newClient = new Client;
    newClient->id = rand(); // TODO: Replace
    newClient->profile.id = -1;
    newClient->profile.roles = -1;
    newClient->profile.name = name;

    printf("Guest login: %s\n", newClient->profile.name.c_str());

    clients[newClient->id] = newClient;
    return newClient;
}

void client_remove(Client* client){
    clients.erase(client->id);
    delete client;
}

// Broadcasting
void room_list(SendBuffer& buffer){
    buffer.Write<int>(rooms.size());

    for (const auto [id, room] : rooms){
        buffer.Serialize<RoomInfo>(room->info);
    }
}

void player_list(SendBuffer& buffer){
    buffer.Write<int>(clients.size());

    for (const auto [id, client] : clients){
        buffer.Serialize<Profile>(client->profile);
    }
}


void send_login_reject(SendBuffer& sendBuffer, ENetPeer* peer, std::string reason){
    sendBuffer.Begin(Message::LOGIN_FAIL);
    sendBuffer.WriteString("User accounts have not been implemeted yet.");
    send_buffer(peer, sendBuffer);
    sendBuffer.Reset();
}

void send_login_success(SendBuffer& sendBuffer, ENetPeer* peer){
    sendBuffer.Begin(Message::LOGIN_SUCCESS);
    room_list(sendBuffer);
    player_list(sendBuffer);
    send_buffer(peer, sendBuffer);
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


    Network::SendBuffer sendBuffer;
    sendBuffer.buffer = new u8[2048];

    ENetEvent event;
    while (true){
        auto start = std::chrono::high_resolution_clock::now();
        
        while (enet_host_service(server, &event, 0) > 0){
        switch (event.type){
            case ENET_EVENT_TYPE_CONNECT: {


                // printf("Created Client for: %s (%u)\n", client->info.profile.name.c_str(), client->info.id);
            } break;
            
            case ENET_EVENT_TYPE_RECEIVE: {
                Client* client = (Client*)(event.peer->data);
                
                ReadBuffer::Set_Buffer(event.packet->data);
                Message what = ReadBuffer::Read<Message>();
                
                switch (what) {
                    case Message::LOGIN_GUEST: {
                        std::string guestName = ReadBuffer::ReadString();
                        event.peer->data = client_create_guest(guestName);

                        send_login_success(sendBuffer, event.peer);
                    } break;
                    case Message::LOGIN_ACCOUNT: {
                        std::string username = ReadBuffer::ReadString();
                        std::string password = ReadBuffer::ReadString();

                        send_login_reject(sendBuffer, event.peer, "User accounts not yet implemented.");
                    } break;
                }

                enet_packet_destroy (event.packet);
            } break;

            case ENET_EVENT_TYPE_DISCONNECT: {
                // Client* client = static_cast<Client*>(event.peer->data);
                // clients.erase(client->info.id);
                // printf("Client Disconnected: %s (%u)\n", client->info.profile.name.c_str(), client->info.id);
                // delete client;

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
    Network::room_create("Test room",0,0);
    Network::room_create("Test room2",0,0);

    // Network::ReadBuffer::Set_Buffer(sendBuffer.buffer);

    Network::service();

    // int count = Network::ReadBuffer::Read<int>();
    // printf("%i Room\n", count);
    // for (int i=0; i < count; i++){
    //     std::string name = Network::ReadBuffer::ReadString();
    //     printf("%s\n", name.c_str());
    // }


    // printf("Buffer: %s\n", buff.str().c_str());
    
    return 0;
}