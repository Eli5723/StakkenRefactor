#include <Input/Keyboard.h>
#include <iostream>
#include <UI/NodeTypes.h>

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

UI::Node* Keyboard::create_config_window(){
        UI::Node* root = new UI::Container;
        InputProfile* profile = this->profile;

        for (int i = 0; i < 8; i++){

            auto* label = new UI::Label(SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)profile->keys[i])));

            auto* icon = new UI::Image(("./resources/textures/" + UI::icons[i]).c_str());
            icon->size = {32,32};
            icon->clickCallback = [icon, label](int x, int y){
                UI::SetFocus(icon);
                label->setText("...");
            };
            icon->keyCallback = [i, label, profile](const SDL_KeyboardEvent& event){
                label->setText(SDL_GetKeyName(SDL_GetKeyFromScancode(event.keysym.scancode)));
                profile->keys[i] = event.keysym.scancode;
                UI::ClearFocus();
            };
            root->addChild(icon);
            root->addChild(label);
        }

        auto* icon = new UI::Image("./resources/textures/das.png");
        icon->size = {32,32};
        auto* input = new UI::NumericInput([this](int val){
            this->profile->das = val;
        }, profile->das);
        root->addChild(icon);
        root->addChild(input);

        icon = new UI::Image("./resources/textures/arr.png");
        icon->size = {32,32};
        input = new UI::NumericInput([this](int val){
            this->profile->arr = val;
        }, profile->arr);
        root->addChild(icon);
        root->addChild(input);

        icon = new UI::Image("./resources/textures/droparr.png");
        icon->size = {32,32};
        input = new UI::NumericInput([this](int val){
            this->profile->dropArr = val;
        }, profile->dropArr);
        root->addChild(icon);
        root->addChild(input);

        root->gridLayout(2);
        return root;
    }

    void Keyboard::save_config_window(){
        profile->save();
    }

}