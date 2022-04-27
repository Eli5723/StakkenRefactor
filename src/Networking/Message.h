#pragma once

#include <types.h>

namespace Network {

enum class Message : u8 {
    LOGIN_ACCOUNT,
    LOGIN_GUEST,

    LOGIN_SUCCESS,
    LOGIN_FAIL,

    PLAYER_CONNECT,
    PLAYER_DISCONNECT,
    
    ROOM_LIST,
    
    ROOM_CREATE,
    ROOM_CREATE_SUCCESS,
    ROOM_CREATE_FAIL,

    ROOM_CREATED,
    ROOM_REMOVED,

    ROOM_JOIN,
    ROOM_JOIN_SUCCESS,
    ROOM_JOIN_FAIL
};

enum Roles {
    GUEST = 0,
    USER = 1
};

}