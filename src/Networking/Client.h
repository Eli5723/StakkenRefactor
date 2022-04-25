#pragma once

#include <string>

namespace Network {

struct RoomInfo {
    int id;
    std::string name;
};

struct PlayerInfo {
    int id;
    int roles;
    std::string name;
};

void send_login_request(const std::string& username, const std::string& password);
void send_guest_request(const std::string& nickname);

void send_room_request(const std::string& nickname);

void connect();

}