#pragma once

#include <UI/UI.h>

namespace UI
{

struct HiddenInput : public Node {
    const int SIZE = 255;
    char* value;
    u8 cursor;
    bool editing;
    std::function<void(char*)> callback;

    HiddenInput(std::function<void(char*)> callback);
    ~HiddenInput();

    void render();
};

}