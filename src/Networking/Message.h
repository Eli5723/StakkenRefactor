#pragma once

#include <types.h>

namespace Network {

enum class Message : u8 {
    LOGIN_ACCOUNT,
    LOGIN_GUEST,

    LOGIN_SUCCESS,
    LOGIN_FAIL,
    
    ROOM_LIST,
    ROOM_CREATE,
    ROOM_JOIN,
};

enum Roles {
    GUEST = 0,
    USER = 1
};

}