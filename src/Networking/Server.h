#pragma once

#include <enet/enet.h>
#include <string>

struct Client {
    int id;
    int roles;

    std::string name;

    ENetPeer* socket;
};