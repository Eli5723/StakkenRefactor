#pragma once

#include <Networking/Room.h>
#include <Networking/Profile.h>

#include <string>


namespace Network {

struct Room;

struct Client {
    int id;
    Profile profile;

    Room* room;
};

}