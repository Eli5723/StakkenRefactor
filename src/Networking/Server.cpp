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

// Rooms
Room* room_create(const std::string_view& name, int creatorId, int gamemode){
    Room* newRoom = new Room;
    newRoom->name = name;
    newRoom->id = rand(); // TODO: Replace

    printf("Created new room: %s\n", newRoom->name.c_str());

    rooms[newRoom->id] = newRoom;
    return newRoom;
}

void room_remove(Room* room){
    rooms.erase(room->id);

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
        buffer.WriteString(room->name.c_str());
    }
}

inline void send_buffer(ENetPeer* peer, SendBuffer& buffer){
    ENetPacket* packet = enet_packet_create(buffer.GetBuffer(), buffer.GetBufferSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(server);
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

                Client* client = client_create_guest("Guest");
                event.peer->data = client;

                room_list(sendBuffer);
                send_buffer(event.peer, sendBuffer);
                sendBuffer.Reset();

                // printf("Created Client for: %s (%u)\n", client->info.profile.name.c_str(), client->info.id);
            } break;
            
            case ENET_EVENT_TYPE_RECEIVE: {
                // Client* client = static_cast<Client*>(event.peer->data);
                
                // ReadBuffer::Set_Buffer(event.packet->data);
                // MessageDestination where = ReadBuffer::Read<MessageDestination>();
                
                // switch (where){
                //     case MessageDestination::Server:
                //         Process_Server_Message(client);
                //     break;
                //     case MessageDestination::Room:
                //         if (client->room)
                //             client->room->Process_Room_Message(client);
                //     break;
                // }

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