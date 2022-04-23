#pragma once

#include <UI/UI.h>

namespace UI {

    struct NumericInput : public Node {
        const int SIZE = 11; // Max amount of space for an integer
        char* value;
        int lastValue;
        bool allowNegative = true;

        u8 cursor;
        bool editing;
        std::function<void(int)> callback;

        NumericInput(std::function<void(int)> callback, int initialValue);
        ~NumericInput();
        inline void SetValue(int toSet);

        void render();
    };

}