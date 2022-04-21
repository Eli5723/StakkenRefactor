#pragma once

namespace Network {

enum class Message {
    LOGIN_ACCOUNT,
    LOGIN_GUEST,

    LOGIN_SUCCESS,
    LOGIN_FAIL,
    
    ROOM_LIST,
    ROOM_CREATE,
    ROOM_JOIN,
};

}