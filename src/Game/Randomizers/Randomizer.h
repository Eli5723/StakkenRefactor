#pragma once

#include <types.h>

class Randomizer {
    virtual u64 next()=0;
};