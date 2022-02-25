#pragma once
#include <Lobby.h>

struct Lobby;
struct LobbyContext {
    Lobby* lobby;
    virtual void on_add(){}
    virtual void on_remove(){}
};