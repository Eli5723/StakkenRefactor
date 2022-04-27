#pragma once

#include <types.h>

#include <string>
#include <mutex>
#include <unordered_map>

namespace Network {

struct RoomInfo {
    u32 session;
    std::string name;
};

struct PlayerInfo {
    u32 session;
    u32 id;
    u32 roles;
    std::string name;
};

void send_login_request(const std::string& username, const std::string& password);
void send_guest_request(const std::string& nickname);

void send_room_create(const std::string& nickname);
void send_room_join(const std::string& nickname);

void connect();

extern bool rooms_dirty;
extern std::mutex rooms_mut;
extern std::unordered_map<u32, RoomInfo> rooms;

extern bool players_dirty;
extern std::mutex players_mut;
extern std::unordered_map<u32, PlayerInfo> players;

}