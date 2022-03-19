#include <Input/Input.h>

#include <vector>

using std::vector;

namespace Input {

    vector<IController*> controllers;
    vector<Keyboard*> keyboards;

    void Update(int dt){
        for (auto controller : controllers){
            controller->update(dt);
        }
    }

    void KeyEvent(const SDL_KeyboardEvent& keyEvent){
        SDL_Scancode scancode = keyEvent.keysym.scancode;
        
        for (auto keyboard : keyboards){
            keyboard->keyEvents(scancode);
        }
    }

    void Clear(){
        for (auto controller : controllers){
            controller->clear();
        }
    }

    IController* CreateKeyboard(){
        Keyboard* newKeyboard = new Keyboard();
        newKeyboard->profile = InputProfile::Load("Default");

        controllers.push_back(newKeyboard);
        keyboards.push_back(newKeyboard);

        return newKeyboard;
    }

    void RemoveKeyboard(Keyboard* keyboard){
        auto loc = std::find(keyboards.begin(), keyboards.end(), keyboard);
        if (loc != keyboards.end())
            keyboards.erase(loc);

        auto contloc = std::find(controllers.begin(), controllers.end(), keyboard);
        if (contloc != controllers.end())
            controllers.erase(contloc);

        delete keyboard;
    }
}