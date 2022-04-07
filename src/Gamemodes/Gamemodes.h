#pragma once

#include <Game/Game.h>

struct VS : GameRules {
    virtual int get_id(){return -1;}

    virtual void on_init(Game* game){};
    virtual void on_update(Game* game, int dt) = 0;
    virtual void on_place(Game* game,int cleared, int sent) = 0;
    virtual void on_win(Game* game){};
    virtual void on_lose(Game* game){};
    virtual int score(Game* game){return 0;}; 
    virtual bool result(Game* game){return 0;};

    virtual void score_format(int score, char*){};

    virtual bool compare(int score_a, int score_b){
        return score_a > score_b;
    }
};