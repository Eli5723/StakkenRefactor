#pragma once

#include <Game/Demo.h>
#include <Game/Game.h>

struct DemoPlayer {
    Game target;
    Demo* demo;
    
    int time = 0;
    float speed = 10;

    void advance(int dt);
    void rewind(int dt);
    void restart();
    void pause();
};