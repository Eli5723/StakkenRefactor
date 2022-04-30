#pragma once

#include <SFMLHelper.h>
#include <enet/enet.h>
#include <string>

#include <types.h>

#include <unordered_map>

#include <Networking/Message.h>

struct Room;
struct Client {
    u32 session;
    u32 id;
    u32 roles;
    std::string name;

    ENetPeer* socket;
    Room* room;

    void Encode(sf::Packet& pk){
        pk << session;
        pk << id;
        pk << roles;
        pk << name;
    }
};

struct Room {
    u32 session;
    std::string name;

    std::unordered_map<u32, Client*> clients;

    void Encode(sf::Packet& pk){
        pk << session;
        pk << name;
    }

    void AddClient(Client* client){
        clients[client->id] = client;
    }

    void RemoveClient(Client* client){
        clients.erase(client->id);
    }
};