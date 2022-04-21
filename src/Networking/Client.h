#pragma once

#include <Networking/Room.h>
#include <Networking/Profile.h>

#include <string>
#include <enet/enet.h>

namespace Network {

struct Room;

struct Client {
    ENetPeer* peer;

    int id;
    Profile profile;

    Room* room;
};

}