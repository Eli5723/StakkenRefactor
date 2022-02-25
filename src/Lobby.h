#pragma once

#include <map>

#include <Player.h>

#include <UI/NodeTypes.h>

#include <LobbyContext.h>

struct Lobby {
    std::map<int, Player*> players;
    void player_add(Player* player);
    void player_remove(Player* player);

    LobbyContext* context;
    void set_context(LobbyContext*);
};