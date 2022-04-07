#pragma once

#include <Networking/Room.h>

#include <string>

namespace Network {

struct Room;

struct Profile {
    int id;
    int roles;
    std::string name;
};

struct Client {
    int id;
    Profile profile;

    Room* room;
};

}