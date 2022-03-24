#pragma once

#include <Application.h>

struct SettingsState : public State {
    void init();
    void update(int dt, int time);
    void render(int dt, int time);
    void close();
    bool key_capture(const SDL_KeyboardEvent& key);
};