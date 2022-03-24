#include <Input/Keyboard.h>
#include <iostream>

namespace Input {

void Keyboard::keyEvents(SDL_Scancode input){

    if (input==profile->rcw) { 
        buffer.queue(TetrisEvent::RCW);
        return;
    }

    if (input==profile->rccw) {
        buffer.queue(TetrisEvent::RCCW);
        return;
    }

    if (input==profile->flip) {
        buffer.queue(TetrisEvent::Flip);
        return;
    }

    if (input==profile->hardDrop) {
        buffer.queue(TetrisEvent::HardDrop);
        return;
    }
    
    if (input==profile->sonicDrop) {
        buffer.queue(TetrisEvent::SonicDrop);
        return;
    }

    // Repeatable actions, set the repeatable values
    if (input==profile->left) {
        repeatKey = profile->left;
        buffer.queue(TetrisEvent::Left);
        lastEvent = TetrisEvent::Left;

        timeToRepeat = profile->das;
        return;
    }
    
    if (input==profile->right) {
        repeatKey = profile->right;
        buffer.queue(TetrisEvent::Right);
        lastEvent = TetrisEvent::Right;

        timeToRepeat = profile->das;
        return;
    }

    if (input==profile->softDrop) {
        buffer.queue(TetrisEvent::SoftDrop);

        timeToRepeatDrop = profile->dropArr;
        return;
    }
}

void Keyboard::update(int timeElapsed){
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    int maxActions = 9;

    // Handle Soft Drop auto repeat
    if (keys[profile->softDrop]){
        timeToRepeatDrop-=timeElapsed;
        while (timeToRepeatDrop < 0 && maxActions--){
            timeToRepeatDrop += profile->dropArr;
            buffer.queue(TetrisEvent::SoftDrop);
        }
    }

    if (lastEvent != TetrisEvent::None && keys[repeatKey]) {
        timeToRepeat-=timeElapsed;

        // Handle Side auto repeat
        while (timeToRepeat < 0 && maxActions--){
            timeToRepeat += profile->arr;
            buffer.queue(lastEvent);
        }
    }
}

void Keyboard::clear(){
    buffer.clear();
}

EventBuffer* Keyboard::getEvents(){
    return &buffer;
}

}