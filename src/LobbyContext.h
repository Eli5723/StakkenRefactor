#pragma once
#include <Game/Game.h>

struct Lobby;
struct IMultiplayerContext;

struct LobbyContext {
    IMultiplayerContext* lobby;
    
    virtual void on_add(){}
    virtual void on_remove(){}
};