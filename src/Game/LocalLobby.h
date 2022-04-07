#pragma once

#include <Player.h>

#include <Game/Game.h>
#include <Game/Randomizers/XoroshiroRandomizer.h>

struct LocalLobby : IMultiplayerContext {
    XoroshiroRandomizer gameRandomizer;

    void player_add(Player* player);
    void player_remove(Player* player);

    void seed(int seed);
    void send_lines(Game* source, int count);
    void on_player_loss();
    void on_player_win();
};