#pragma once 

#include <Player.h>
#include <Game/Game.h>

struct IMultiplayerContext {
    LobbyContext* context = 0;
    std::map<int, Player*> players;
    std::map<int, Game*> games;

    virtual void seed(int seed) = 0;
    virtual void send_lines(Game* source) = 0;
    virtual void on_player_loss(Game* game) = 0;
    virtual void on_player_win(Game* game) = 0; 
};