#pragma once
#include <Game/EventBuffer.h>
#include <UI/Node.h>

namespace  Input {

    struct IController {
        virtual void update(int dt) = 0;
        virtual EventBuffer *getEvents() = 0;
        virtual void clear() = 0;

        // Each input type should be able to generate a custom configuration window
        virtual UI::Node* create_config_window() = 0;
        virtual void save_config_window() = 0;
    };

}