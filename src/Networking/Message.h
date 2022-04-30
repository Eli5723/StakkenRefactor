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
    ROOM_JOIN_FAIL,

    ROOM_OTHER_JOINED,
    ROOM_OTHER_LEFT,
    ROOM_SLOT_GRANT,
    ROOM_SLOT_REMOVE,
    ROOM_REMOVED_YOU,
    ROOM_CHAT_MESSAGE,
    ROOM_GAME_START,
};

enum Roles {
    GUEST = 0,
    USER = 1
};

}