#include <Networking/Client.h>

#include <Networking/Message.h>
#include <enet/enet.h>
#include <iostream>
#include <chrono>

#include <thread>
#include <mutex>
#include <rigtorp/SPSCQueue.h>

#include <SFMLHelper.h>

#include <Application.h>
#include <States/LobbyState.h>

namespace Network {

bool running = false;


// Network Members
ENetAddress address;
ENetHost* client;
ENetPeer* server;

static int server_time;
static int server_time_diff;
static int ping_sent;
static int ping_recieved;
static int ping;

// Network State
PlayerInfo localPlayer;


// Packet Sending
rigtorp::SPSCQueue<sf::Packet> async_queue(10);

void send_async(sf::Packet&& packet){
    async_queue.push(std::move(packet));
}

void send(const sf::Packet& packet, const u8 channel = 0)
{
  ENetPacket* epacket = enet_packet_create(packet.getData(), packet.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(server, channel, epacket);
}

void send_login_request(const std::string& username, const std::string&  password){
    sf::Packet packet;

    packet << Message::LOGIN_ACCOUNT;
    packet << username;
    packet << password;

    async_queue.push(packet);
}

void send_guest_request(const std::string& nickname){
    sf::Packet packet;

    packet << Message::LOGIN_GUEST;
    packet << nickname;

    async_queue.push(packet);
}

void send_room_request(const std::string& name){
    sf::Packet packet;

    packet << Message::ROOM_CREATE;
    packet << name;

    async_queue.push(packet);
}

// Packet Handling
void OnData(sf::Packet& packet){
    Message m;

    packet >> m;

    switch (m) {
        case Message::LOGIN_FAIL:
            puts("Login failed!");
        break;

        case Message::LOGIN_SUCCESS:
            packet >> localPlayer.id;
            packet >> localPlayer.roles;
            packet >> localPlayer.name;

            Application::instance->state_set(new LobbyState);

            printf("Successfully logged in as %s (%i | %i)\n", localPlayer.name.c_str(), localPlayer.id, localPlayer.roles);
        break;
    
        case Message::ROOM_CREATE:
            puts("Login succeeded!");
        break;        

        case Message::ROOM_LIST:
            puts("Login succeeded!");
        break;        

        default:
            puts("Recieved Invalid Message from server.");
        break;
    }
}

void OnLoginSuccess(sf::Packet& packet){
    packet >> localPlayer.id;
    packet >> localPlayer.roles;
    packet >> localPlayer.name;

    printf("Successfully Logged in as %s (%i | %i)\n");
}

void service(){

    ENetEvent event{};

    while (true){

        // Prepare all queued packets to be sent
        while (async_queue.front()){
            auto& packet = *(async_queue.front());
            send(packet);
            puts("sendin'");
            async_queue.pop();
        }

        while (enet_host_service(client, &event, 250) > 0){
            // Handle incoming messages
            sf::Packet incoming;
            
            switch (event.type){
                case ENET_EVENT_TYPE_RECEIVE:{
                    sf::Packet incoming;
                    incoming.append(event.packet->data,event.packet->dataLength);
                    OnData(incoming);
                    enet_packet_destroy (event.packet);

                } break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    puts("Lost Connection!");
                    // TODO: Implement
                break;
            }
        }
    }
}

// Servicing connections
void connect(){
    if (running){
        puts("Attempted to connect while already running.");
        return;
    }

    running = true;
    
    client = enet_host_create (NULL, 1, 2, 0, 0);
    if (client == NULL)
    {
        fprintf (stderr, "An error occurred while trying to create an ENet client host.\n"); exit (EXIT_FAILURE);
    }

    ENetEvent event;

    // Initial Connection
    enet_address_set_host(&address, "localhost");
    address.port = 1234;

    server = enet_host_connect(client, &address, 2, 0);
    puts("Attempting to connect\n");     
    if (enet_host_service(client, &event, 5000) > 0) {
        puts("Successfully Connected\n");        
        std::thread netThread([](){service();});
        netThread.detach();
    } else {
        fprintf(stderr, "Failed to connect.\n");
        running = false;
    }
    
}

}