#pragma once

#include <SDL2/SDL.h>

#include <Input/IController.h>

#include <Game/EventBuffer.h>

#include <Input/InputProfile.h>
#include "InputProfile.h"

namespace Input {

struct KeyboardBuffer : EventBuffer {
    KeyboardBuffer(){
        events = new TetrisEvent[20];
        count = 0;
    }

    ~KeyboardBuffer(){
        delete [] events;
    }

    void queue(TetrisEvent event){
        if (count < 20) {
            events[count++] = event;
        }

    }

    void clear(){
        count = 0;
    }
};

struct Keyboard : IController {
    InputProfile* profile;
    KeyboardBuffer buffer;

    TetrisEvent lastEvent = TetrisEvent::None;
    u8 repeatKey;
    int timeToRepeat;
    int timeToRepeatDrop;
    bool SoftDropping;

    void keyEvents(SDL_Scancode input);
    void update(int timeElapsed);
    void clear();
    EventBuffer* getEvents();

    // Config editing
    UI::Node* create_config_window();
    void save_config_window();

};

}