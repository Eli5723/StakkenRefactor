#pragma once
#include <Application.h>

struct RoomSelectState : public State {
    void init();
    void update(int dt, int time);
    void render(int dt, int time);
    void close();
};