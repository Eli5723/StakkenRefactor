#pragma once

namespace Input {

    struct IController {
        virtual void update(int dt) = 0;
        virtual EventBuffer *getEvents() = 0;
        virtual void clear() = 0;
    };


}
