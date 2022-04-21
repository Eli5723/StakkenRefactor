#pragma once

#include <Networking/Client.h>

#include <Networking/ReadBuffer.h>
#include <Networking/SendBuffer.h>

#include <map>
#include <string>

namespace Network {

struct Client;



struct Room {
    RoomInfo info;

    std::map<int, Client*> clients;
};

}