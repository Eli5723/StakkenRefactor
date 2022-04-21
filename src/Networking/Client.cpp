#include <Networking/ReadBuffer.h>
#include <Networking/SendBuffer.h>
#include <Networking/Message.h>

#include <enet/enet.h>

#include <Networking/Profile.h>

#include <iostream>
#include <chrono>

#include <thread>
#include <queue>

namespace Network {

ENetAddress address;
ENetHost* client;
ENetPeer* peer;

static int server_time;
static int server_time_diff;
static int ping_sent;
static int ping_recieved;
static int ping;

u8 buffer[2048];

std::mutex rooms_mutex;
std::queue<RoomInfo> rooms;

std::mutex profiles_mutex;
std::queue<Profile> profiles;

// Recieving Messages 
void update_room_list(){
    rooms_mutex.lock();

    int count = ReadBuffer::Read<int>();
    printf("Got %i rooms.\n", count);
    
    for (int i=0; i < count; i++){
        RoomInfo room;
        ReadBuffer::Deserialize<RoomInfo>(room);
        rooms.push(room);

        printf("%s (%i)\n", room.name.c_str(), room.id);
    }
    putc('\n',stdout);

    rooms_mutex.unlock();
}
 
void update_player_list(){

    profiles_mutex.lock();
    int count = ReadBuffer::Read<int>();
    printf("Got %i players.\n", count);
    
    for (int i=0; i < count; i++){
        Profile profile;
        ReadBuffer::Deserialize<Profile>(profile);
        profiles.push(profile);

        printf("%s (%i:%i)\n", profile.name.c_str(), profile.id, profile.roles);
    }
    putc('\n',stdout);

    profiles_mutex.unlock();
}

// Sending Messages
void sendBuffer(SendBuffer& buffer){
    ENetPacket* packet = enet_packet_create(buffer.GetBuffer(), buffer.GetBufferSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);
}

void Login_Guest(SendBuffer& buffer, const std::string_view& desiredName){
    buffer.Write<Message>(Message::LOGIN_GUEST);
    buffer.WriteString(desiredName.cbegin());
    sendBuffer(buffer);
}

void Login_Account(SendBuffer& buffer, const std::string_view& username, const std::string_view& password){
    // TODO: IMPLEMENT & MAKE THIS SECURE
    buffer.Write<Message>(Message::LOGIN_ACCOUNT);
    buffer.WriteString(username.cbegin());
    buffer.WriteString(password.cbegin());
    sendBuffer(buffer);
}

void room_create(SendBuffer& buffer, const char* name, int gamemode){
    buffer.Begin(Message::ROOM_CREATE);
    buffer.WriteString(name);
    buffer.Write<int>(gamemode);
    sendBuffer(buffer);
}

// Servicing connections
int connect(){
    client = enet_host_create (NULL, 1, 2, 0, 0);
    if (client == NULL)
    {
        fprintf (stderr, "An error occurred while trying to create an ENet client host.\n"); exit (EXIT_FAILURE);
    }

    ENetEvent event;

    // Initial Connection
    enet_address_set_host(&address, "localhost");
    address.port = 1234;

    peer = enet_host_connect(client, &address, 2, 0);
    puts("Attempting to connect\n");     
    if (enet_host_service(client, &event, 5000) > 0) {
        puts("Successfully Connected\n");        
    } else {
        fprintf(stderr, "Failed to connect.\n");
        return 1;
    }

    SendBuffer sendBuffer;
    sendBuffer.buffer = &buffer[0];

    Login_Guest(sendBuffer, "username");

    // Login_Account(sendBuffer, "username", "password");

    ///Event Servicing 
    while (true){
        auto start = std::chrono::high_resolution_clock::now();

        while (enet_host_service(client, &event, 0) > 0){
        switch (event.type){
            case ENET_EVENT_TYPE_RECEIVE:{
                ReadBuffer::Set_Buffer(event.packet->data);
                Message what = ReadBuffer::Read<Message>();
                
                switch (what){
                    case Message::LOGIN_SUCCESS: {
                        update_room_list();
                        update_player_list();

                        room_create(sendBuffer, "My lobby", 0);
                    } break;

                    case Message::LOGIN_FAIL: {
                        std::string reason = ReadBuffer::ReadString();
                        printf("Failed to login: %s\n", reason.c_str());
                    } break;

                    case Message::ROOM_LIST:
                        update_room_list();
                    break;
                }
                enet_packet_destroy (event.packet);
            } break;
        }}

        std::this_thread::sleep_until(start + std::chrono::milliseconds(16));
    }
}

} // namespace Network




int main() {

    
    Network::connect();

    
    return 0;
}