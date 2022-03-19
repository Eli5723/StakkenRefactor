#include <Input/IController.h>

#include <Input/Keyboard.h>
#include <Input/InputProfile.h>

namespace Input {
    void Update(int dt);

    void Clear();

    void KeyEvent(const SDL_KeyboardEvent& keyEvent);

    IController* CreateKeyboard();
    void RemoveKeyboard(Keyboard*);
}