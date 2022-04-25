#pragma once

#include <types.h>

#include <enet/enet.h>

namespace Network {

struct Room;

struct Peer {
    ENetPeer* peer;

    u32 id;
    u32 ping;

    Room* room;
};

}