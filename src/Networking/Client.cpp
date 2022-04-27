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
#include <States/RoomSelectState.h>

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

    std::mutex players_mut;
    bool players_dirty = false;
    std::unordered_map<u32, PlayerInfo> players;

    bool rooms_dirty = false;
    std::mutex rooms_mut;
    std::unordered_map<u32, RoomInfo> rooms;

    // Packet Sending
    rigtorp::SPSCQueue<sf::Packet> async_queue(10);

    void send_async(sf::Packet&& packet) {
        async_queue.push(std::move(packet));
    }

    void send(const sf::Packet& packet, const u8 channel = 0)
    {
        ENetPacket* epacket = enet_packet_create(packet.getData(), packet.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(server, channel, epacket);
    }

    void send_login_request(const std::string& username, const std::string& password) {
        sf::Packet packet;

        packet << Message::LOGIN_ACCOUNT;
        packet << username;
        packet << password;

        async_queue.push(packet);
    }

    void send_guest_request(const std::string& nickname) {
        sf::Packet packet;

        packet << Message::LOGIN_GUEST;
        packet << nickname;

        async_queue.push(packet);
    }

    void send_room_create(const std::string& name) {
        sf::Packet packet;

        packet << Message::ROOM_CREATE;
        packet << name;

        async_queue.push(packet);
    }

    // Packet Handling
    void OnLoginSuccess(sf::Packet& packet) {
        packet >> localPlayer.session;
        packet >> localPlayer.id;
        packet >> localPlayer.roles;
        packet >> localPlayer.name;

        players[localPlayer.session] = localPlayer;

        Application::instance->state_set(new RoomSelectState());
        printf("Successfully Logged in as %s (%i | %i)\n", localPlayer.name.c_str(), localPlayer.id, localPlayer.roles);
    }

    // Packet Handling
    void OnPlayerConnect(sf::Packet& packet) {
        std::scoped_lock m(players_mut);

        PlayerInfo playerInfo;
        packet >> playerInfo.session;
        packet >> playerInfo.id;
        packet >> playerInfo.roles;
        packet >> playerInfo.name;
        players[playerInfo.session] = playerInfo;

        players_dirty = true;
    }

    // Packet Handling
    // Another player has disconnected
    void OnPlayerDisconnect(sf::Packet& packet) {
        std::scoped_lock m(players_mut);

        u32 session;
        packet >> session;

        players.erase(session);
        players_dirty = true;
    }

    // Full list of all online players
    void OnPlayerList(sf::Packet& packet) {
        std::scoped_lock m(players_mut);

        u8 count;
        packet >> count;

        while (count--) {
            PlayerInfo playerInfo;
            packet >> playerInfo.session;
            packet >> playerInfo.id;
            packet >> playerInfo.roles;
            packet >> playerInfo.name;
            players[playerInfo.session] = playerInfo;
        }

        players_dirty = true;
        puts("Successfully updated online player list.");
    }

    // Packet Handling
    void OnRoomList(sf::Packet& packet) {
        std::scoped_lock m(rooms_mut);

        u8 count;
        packet >> count;

        while (count--) {
            RoomInfo roomInfo;
            packet >> roomInfo.session;
            packet >> roomInfo.name;
            rooms[roomInfo.session] = roomInfo;
        }

        rooms_dirty = true;
    }

    // Another player has created a room
    void OnRoomCreate(sf::Packet& packet){
        std::scoped_lock m(rooms_mut);
        RoomInfo roomInfo;
        packet >> roomInfo.session;
        packet >> roomInfo.name;
        rooms[roomInfo.session] = roomInfo;

        rooms_dirty = true;
    }

    // We have created a room; join it
    void OnRoomCreateSuccess(sf::Packet& packet){
        std::scoped_lock m(rooms_mut);

        RoomInfo roomInfo;
        packet >> roomInfo.session;
        packet >> roomInfo.name;
        rooms[roomInfo.session] = roomInfo;

        rooms_dirty = true;
    }

    void OnData(sf::Packet& packet) {
        Message m;

        packet >> m;

        printf("Message: %c\n", ((char)m + 'a'));

        switch (m) {
        case Message::LOGIN_FAIL:
            enet_peer_reset(server);
            puts("Login failed!");
            break;

        case Message::LOGIN_SUCCESS:
            OnLoginSuccess(packet);
            OnPlayerList(packet);
            OnRoomList(packet);
            break;

        case Message::PLAYER_CONNECT:
            puts("Recieved new player.");
            OnPlayerConnect(packet);
            break;

        case Message::PLAYER_DISCONNECT:
            puts("A player has disconnected.");
            OnPlayerDisconnect(packet);
            break;


        case Message::ROOM_CREATE:
            puts("Someone created a room!");
            OnRoomCreate(packet);
            break;

        
        case Message::ROOM_CREATE_SUCCESS:
            puts("Created a room successfully!");
            OnRoomCreate(packet);
            break;

        case Message::ROOM_LIST:
            puts("Login succeeded!");
            OnRoomList(packet);
            break;

        default:
            puts("Recieved Invalid Message from server.");
            break;
        }
    }

    void service() {

        ENetEvent event{};

        while (true) {

            // Prepare all queued packets to be sent
            while (async_queue.front()) {
                auto& packet = *(async_queue.front());
                send(packet);
                puts("sendin'");
                async_queue.pop();
            }

            while (enet_host_service(client, &event, 250) > 0) {
                // Handle incoming messages
                sf::Packet incoming;

                switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {
                    sf::Packet incoming;
                    incoming.append(event.packet->data, event.packet->dataLength);
                    OnData(incoming);
                    enet_packet_destroy(event.packet);

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
    void connect() {
        if (running) {
            puts("Attempted to connect while already running.");
            return;
        }

        running = true;

        client = enet_host_create(NULL, 1, 2, 0, 0);
        if (client == NULL)
        {
            fprintf(stderr, "An error occurred while trying to create an ENet client host.\n"); exit(EXIT_FAILURE);
        }

        ENetEvent event;

        // Initial Connection
        enet_address_set_host(&address, "localhost");
        address.port = 1234;

        server = enet_host_connect(client, &address, 2, 0);
        puts("Attempting to connect\n");
        if (enet_host_service(client, &event, 5000) > 0) {
            puts("Successfully Connected\n");
            std::thread netThread([]() {service();});
            netThread.detach();
        }
        else {
            fprintf(stderr, "Failed to connect.\n");
            running = false;
        }

    }

}