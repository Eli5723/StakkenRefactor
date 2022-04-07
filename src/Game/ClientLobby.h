#pragma once

#include <Game/Game.h>

struct AttackRecord;

struct OnlineLobby : IMultiplayerContext {
    void seed(int seed);
    void send_lines(Game*);
    void send_lines(Game*, int amount);
    void on_player_loss();
    void on_player_win();
};