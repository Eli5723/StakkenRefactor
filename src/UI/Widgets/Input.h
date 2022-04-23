#pragma once

#include <UI/UI.h>

namespace UI
{

struct Input : public Node {
    const int SIZE = 255;
    char* value;
    u8 cursor;
    bool editing;
    std::function<void(char*)> callback;

    Input(std::function<void(char*)> callback);
    ~Input();

    void render();
};

}