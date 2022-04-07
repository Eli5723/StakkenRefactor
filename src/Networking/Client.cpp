#include <Networking/ReadBuffer.h>
#include <Networking/SendBuffer.h>
#include <Networking/Message.h>

#include <enet/enet.h>

#include <iostream>
#include <chrono>
#include <thread>

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

void sendBuffer(SendBuffer& buffer){
    ENetPacket* packet = enet_packet_create(buffer.GetBuffer(), buffer.GetBufferSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);
}

void Login_Guest(SendBuffer& buffer, const std::string_view& desiredName){

}

void Login_Account(SendBuffer& buffer, const std::string_view& username, const std::string_view& password){
    // TODO: IMPLEMENT & MAKE THIS SECURE
}


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

    ///Event Servicing 
    while (true){
        auto start = std::chrono::high_resolution_clock::now();

        while (enet_host_service(client, &event, 0) > 0){
        switch (event.type){
            
            case ENET_EVENT_TYPE_RECEIVE:{
                ReadBuffer::Set_Buffer(event.packet->data);
                Message what = ReadBuffer::Read<Message>();
                
                switch (what){
                    case Message::LOGIN_ACCOUNT:
              
                    break;
                }
                printf("Hi\n");

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