#pragma once

#include <Networking/Client.h>

#include <map>
#include <string>

namespace Network {

struct Client;
struct Room {
    int id;
    std::string name;

    std::map<int, Client*> clients;
}; 

}