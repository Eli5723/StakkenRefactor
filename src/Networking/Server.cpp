#include <Networking/Server.h>

#include <Networking/Message.h>

#include <rigtorp/SPSCQueue.h>
#include <SFMLHelper.h>

#include <enet/enet.h>

#include <map>
#include <string_view>
#include <sstream>
#include <chrono>
#include <thread>
#include <unordered_map>


namespace Network {

    std::unordered_map<u32, Room*> rooms;
    std::unordered_map<u32, Client*> clients;

    int sesion_inc;

    ENetAddress address;
    ENetHost* server;

    // Sending
    rigtorp::SPSCQueue<sf::Packet> async_queue(10);

    void send_async(sf::Packet&& packet) {
        async_queue.push(std::move(packet));
    }

    void send(const sf::Packet& packet, ENetPeer* peer, const u8 channel = 0)
    {
        ENetPacket* epacket = enet_packet_create(packet.getData(), packet.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, channel, epacket);
    }

    void sendExcept(const sf::Packet& packet, ENetPeer* peer, const u8 channel = 0)
    {

        ENetPacket* epacket = enet_packet_create(packet.getData(), packet.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
        for (auto& c : clients) {
            auto& client = c.second;

            if (client->socket != peer) {
                enet_peer_send(client->socket, channel, epacket);
            }
        }
    }

    // Encoding Helpers
    void EncodePlayerList(sf::Packet& pk) {
        u8 size = clients.size();

        pk << size;

        for (const auto& player : clients) {
            player.second->Encode(pk);
        }
    }

    void EncodeRoomList(sf::Packet& pk) {
        u8 size = rooms.size();

        pk << size;

        for (const auto& room : rooms) {
            room.second->Encode(pk);
        }
    }

    // Helpers
    u32 session_inc = 0;
    u32 GenerateSessionID() {
        u32 id = session_inc++;

        while (clients.find(id) != clients.end())
            id = session_inc++;

        return id;
    }

    u32 room_inc = 0;
    u32 GenerateRoomSession(){
        u32 id = room_inc++;

        while (rooms.find(id) != rooms.end())
            id = session_inc++;

        return id;
    }

    // Recieving
    void OnLoginGuest(ENetPeer* peer, sf::Packet& packet) {
        std::string nickname;
        packet >> nickname;

        Client* client = new  Client;

        client->session = GenerateSessionID();
        client->id = -1;
        client->name = nickname;
        client->roles = Roles::GUEST;
        client->socket = peer;
        peer->data = client;

        clients[client->session] = client;

        // Inform Client that login was successful, send room and player list
        sf::Packet pk;
        pk << Message::LOGIN_SUCCESS;
        client->Encode(pk);
        EncodePlayerList(pk);
        EncodeRoomList(pk);
        send(pk, peer);

        // Inform Other Clients
        sf::Packet pk2;
        pk2 << Message::PLAYER_CONNECT;
        client->Encode(pk2);
        sendExcept(pk2, client->socket);

        printf("Got guest login: %s\n", nickname.c_str());
    }

    void OnLoginAccount(ENetPeer* peer, sf::Packet& packet) {
        std::string username;
        std::string password;

        packet >> username;
        packet >> password;

        Client* client = new  Client;
        // TODO: add proper accounts
        client->session = GenerateSessionID();
        client->id = -1;
        client->name = username;
        client->roles = Roles::GUEST;
        client->socket = peer;
        peer->data = client;

        clients[client->session] = client;

        // Inform Client
        sf::Packet pk;

        // Inform Client that login was successful, send room and player list
        pk << Message::LOGIN_SUCCESS;
        client->Encode(pk);
        EncodePlayerList(pk);
        EncodeRoomList(pk);
        send(pk, peer);

        // Inform Other Clients
        sf::Packet pk2;
        pk2 << Message::PLAYER_CONNECT;
        client->Encode(pk2);
        sendExcept(pk2, client->socket);

        printf("Got login: %s | %s\n", username.c_str(), password.c_str());
    }

    void OnRoomCreate(Client* client, sf::Packet& packet) {
        if (client->room)
            return;

        // Validate Input
        std::string name;
        packet >> name;

        // Create Room
        Room* newRoom = new Room;
        newRoom->session = GenerateRoomSession();
        newRoom->name = name;
        rooms[newRoom->session] = newRoom;

        // Place client in room

        // Inform Creator
        sf::Packet response;
        response << Message::ROOM_CREATE_SUCCESS;
        newRoom->Encode(response);
        send(response, client->socket);

        // Overwrite header to inform Other Clients
        Message* buf = (Message*)(response.getData());
        buf[0] = Message::ROOM_CREATED;
        sendExcept(response, client->socket);
    }

    // Packet Handling
    void OnData(ENetPeer* peer, sf::Packet& packet) {
        Message m;

        packet >> m;

        // If the user does not have a client object associated yet, only accept login attempts
        if (!peer->data){
            switch (m){
                case Message::LOGIN_GUEST: {
                    OnLoginGuest(peer, packet);
                } break;

                case Message::LOGIN_ACCOUNT: {
                    OnLoginGuest(peer, packet);
                } break;
                default:
                break;
            }
            return;
        }

        Client* client = (Client*)peer->data;

        switch (m) {
        case Message::ROOM_CREATE: {
            OnRoomCreate(client, packet);
        } break;

        case Message::ROOM_JOIN: {

        } break;

        case Message::ROOM_LIST: {
            sf::Packet pk;
            pk << Message::ROOM_LIST;
            EncodeRoomList(pk);
            send(pk,peer);
        } break;


        default:
            puts("Recieved Invalid Message from client.");
            // TODO: Kick Client
            break;
        }
    }


    void service() {
        address.host = ENET_HOST_ANY;
        address.port = 1234;

        server = enet_host_create(&address, 32, 2, 0, 0);

        if (server == NULL)
        {
            fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
            exit(EXIT_FAILURE);
        }

        ENetEvent event;
        while (true) {
            auto start = std::chrono::high_resolution_clock::now();

            while (enet_host_service(server, &event, 0) > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    event.peer->data = nullptr;
                } break;

                case ENET_EVENT_TYPE_RECEIVE: {
                    sf::Packet incoming;
                    incoming.append(event.packet->data, event.packet->dataLength);
                    OnData(event.peer, incoming);
                    enet_packet_destroy(event.packet);
                } break;

                case ENET_EVENT_TYPE_DISCONNECT: {
                    // Remove Session and destroy object
                    Client* client = (Client*)(event.peer->data);

                    if (!client)
                        return;

                    printf("Client %s (%i,%i) disconnected.\n", client->name.c_str(), client->id, client->roles);

                    // Inform other clients
                    sf::Packet pk;
                    pk << Message::PLAYER_DISCONNECT;
                    pk << client->session;
                    sendExcept(pk, client->socket);

                    clients.erase(client->session);
                    delete client;
                } break;
                }
            }
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