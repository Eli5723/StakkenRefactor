#pragma once

#include <Game/TetrisEvent.h>

struct EventBuffer {
    TetrisEvent* events;
    int count = 0;
};