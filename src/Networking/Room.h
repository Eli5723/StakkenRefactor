#pragma once

#include <Networking/Peer.h>


#include <map>
#include <string>

namespace Network {

struct Peer;

struct Room {

    std::map<int, Peer*> clients;
};

}